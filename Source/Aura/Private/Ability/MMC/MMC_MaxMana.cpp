// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/MMC/MMC_MaxMana.h"
#include "Ability/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	//配置SpiritDef,SpiritDef定义了要从什么对象中取什么属性
	SpiritDef.AttributeToCapture = UAuraAttributeSet::GetSpiritAttribute();
	SpiritDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	SpiritDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(SpiritDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//配置快站参数
	FAggregatorEvaluateParameters EvaluateParams;

	EvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	float CapturedSpiritMagnitude = 0.f;
	GetCapturedAttributeMagnitude(SpiritDef, Spec, EvaluateParams, CapturedSpiritMagnitude);
	CapturedSpiritMagnitude = FMath::Max(CapturedSpiritMagnitude, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 CharacterLevel = CombatInterface->GetCharacterLevel();

	//计算并返回实际数值
	return MaxManaConfig.BaseValue + MaxManaConfig.SpiritCoefficient * CapturedSpiritMagnitude + MaxManaConfig.LevelCoefficient * CharacterLevel;
}
