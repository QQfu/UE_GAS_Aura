// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

struct FMaxManaConfig
{
	float BaseValue = 100.f;
	float SpiritCoefficient = 5.f;
	float LevelCoefficient = 20.f;
};

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	//Constructor
	UMMC_MaxMana();

	//用于返回计算结果
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	FGameplayEffectAttributeCaptureDefinition SpiritDef;

private:
	FMaxManaConfig MaxManaConfig;
	
};
