// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Ability/AuraAttributeSet.h"
#include "Ability/Library/AuraAbilitySystemFunctionLibrary.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(Amor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockBonus);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitBonus);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Amor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, AmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockBonus, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitBonus, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
	}
};

static const AuraDamageStatics& GetDamageStatics()
{
	static AuraDamageStatics Instance;
	return Instance;
};

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().AmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().AmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockBonusDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitBonusDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& OwningSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = OwningSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = OwningSpec.CapturedTargetTags.GetAggregatedTags();

	const UAuraCharacterClassInfoAsset* CharacterClassInfoAsset = UAuraAbilitySystemFunctionLibrary::GetDamageCoefficients(TargetAvatar);
	const UCurveTable* DamageCoefficient = CharacterClassInfoAsset->DamageCoefficient;
	check(DamageCoefficient);

	//计算Damage
	//获取Incoming Damage
	const float IncomingDamage = OwningSpec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

	/**
	 * 实现暴击
	 * 1. 捕获CriticalHitChance，CriticalHitBonus，CriticalHitResistance
	 * 2. 利用CriticalHitResistance和CriticalHitBonus计算暴击伤害
	 * 3. 利用CriticalHitChance判定是否暴击
	 * 4. 计算最终暴击后的伤害
	 */
	float CriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitChanceDef, EvaluateParams, CriticalHitChance);
	//Critical Hit Chance不能小于0，不能大于100%
	CriticalHitChance = FMath::Clamp(CriticalHitChance, 0.f, 100.f);

	float CriticalHitBonus = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitBonusDef, EvaluateParams, CriticalHitBonus);

	float CriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitResistanceDef, EvaluateParams, CriticalHitResistance);

	float FixedCriticalHitBonus = 0.f;
	const FRealCurve* CriticalHitCurve = DamageCoefficient->FindCurve(FName("CriticalHitCoefficient"), FString());
	float CriticalHitCoefficient = CriticalHitCurve->Eval(SourceCombatInterface->GetCharacterLevel_Implementation());
	FixedCriticalHitBonus = CriticalHitBonus * (1 - CriticalHitResistance / (CriticalHitResistance + CriticalHitCoefficient));

	const bool bIsCriticalHit = FMath::RandRange(1,100) <= CriticalHitChance;

	float Damage = bIsCriticalHit ? IncomingDamage * (1 + 0.5 + FixedCriticalHitBonus / 100.f) : IncomingDamage;
	
	/**
	 * 实现格挡
	 * 1. 捕获BlockChance
	 * 2. 捕获BlockBonues
	 * 3. 随机是否格挡
	 * 4. 格挡时计算格挡后伤害
	 */
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BlockChanceDef, EvaluateParams, TargetBlockChance);
	//Block Chance不能小于0，不能大于75%
	TargetBlockChance = FMath::Clamp(TargetBlockChance, 0.f, 75.f);

	float TargetBlockBonus = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BlockBonusDef, EvaluateParams, TargetBlockBonus);
	//Block 减伤不能小于20%，不能大于80%
	TargetBlockBonus += 20.f;
	TargetBlockBonus = FMath::Clamp(TargetBlockBonus, 20.f, 80.f);

	//随机是否格挡成功
	const bool bIsBlocked = FMath::RandRange(0, 100) <= TargetBlockChance;

	Damage = !bIsBlocked ? Damage : Damage * (100 - TargetBlockBonus) / 100.f;

	/**
	 * 实现护甲减伤
	 * 1. 捕获Amor和AmorPenetration
	 * 2. 利用Amor和AmorPenetration计算修正护甲值
	 * 2.5. 利用AuraCharacterClassInfoAsset获取护甲减伤的修正参数
	 * 3. 利用修正护甲值计算伤害
	 */
	float AmorMagnitude = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().AmorDef, EvaluateParams, AmorMagnitude);

	float AmorPenetrationMagnitude = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().AmorPenetrationDef, EvaluateParams, AmorPenetrationMagnitude);
	
	float FixedAmor = AmorMagnitude - AmorPenetrationMagnitude;

	float AmorCoefficient = 500.f;
	const FRealCurve* AmorCurve = DamageCoefficient->FindCurve(FName("AmorCoefficient"), FString());
	AmorCoefficient = AmorCurve->Eval(TargetCombatInterface->GetCharacterLevel_Implementation());

	Damage = Damage * (1 - FixedAmor / (FixedAmor + AmorCoefficient));

	const FGameplayModifierEvaluatedData ModifyData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(ModifyData);
}
