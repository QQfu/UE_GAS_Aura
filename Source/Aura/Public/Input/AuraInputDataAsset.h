// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FAuraInputConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const class UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputActionTag;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputConfig> InputConfigAsset;

	//定义公用方法，根据Tag寻找InputAction
	const UInputAction* FindInputActionByTag(const FGameplayTag& InputTag, bool bLogError = false) const;
};
