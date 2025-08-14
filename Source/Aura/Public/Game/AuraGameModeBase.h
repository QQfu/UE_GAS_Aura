// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UAuraCharacterClassInfoAsset;
/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//定义AuraCharacterClassInfoAsset变量，用于在蓝图端指定DataAsset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UAuraCharacterClassInfoAsset> AuraCharacterClassInfoAsset;
};
