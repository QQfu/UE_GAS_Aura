// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/MMC/MMC_MaxHealth.h"
#include "Ability/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//配置VigorDef,VigorDef定义了要从什么对象中取什么属性
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//配置快站参数
	FAggregatorEvaluateParameters EvaluateParams;

	EvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	float CapturedVigorMagnitude = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluateParams, CapturedVigorMagnitude);
	CapturedVigorMagnitude = FMath::Max(CapturedVigorMagnitude, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 CharacterLevel = CombatInterface->GetCharacterLevel();

	//计算并返回实际数值
	return MaxHealthConfig.BaseValue + MaxHealthConfig.VigorCoefficient * CapturedVigorMagnitude + MaxHealthConfig.LevelCoefficient * CharacterLevel;
}
