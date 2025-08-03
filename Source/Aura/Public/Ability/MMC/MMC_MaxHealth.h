// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

struct FMaxHealthConfig
{
	float BaseValue = 100.f;
	float VigorCoefficient = 5.f;
	float LevelCoefficient = 50.f;
};

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	//Constructor
	UMMC_MaxHealth();

	//用于返回计算结果
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	FGameplayEffectAttributeCaptureDefinition VigorDef;

private:
	FMaxHealthConfig MaxHealthConfig;
};
