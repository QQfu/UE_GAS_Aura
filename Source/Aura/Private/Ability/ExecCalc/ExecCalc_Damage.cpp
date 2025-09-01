// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Ability/AuraAttributeSet.h"

struct AuraDamageStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(Amor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockBonus);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Amor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockBonus, Target, false);
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
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockBonusDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& OwningSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = OwningSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = OwningSpec.CapturedTargetTags.GetAggregatedTags();
	// float AmorMagnitude = 0.f;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().AmorDef, EvaluateParams, AmorMagnitude);
	// AmorMagnitude = FMath::Max<float>(0.0f, AmorMagnitude);

	//计算Damage
	//获取Incoming Damage
	const float IncomingDamage = OwningSpec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

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

	float Damage = bIsBlocked ? IncomingDamage : IncomingDamage * (100 - TargetBlockBonus) / 100.f;

	const FGameplayModifierEvaluatedData ModifyData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(ModifyData);
}
