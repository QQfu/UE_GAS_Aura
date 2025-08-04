// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAttributeInfoAsset.generated.h"

/**
 * 用于封装Attribute信息
 */
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription;

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeInfoAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	//用于存储所有Attribute信息的数据结构
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInformation;

	//向外暴露的根据Tag获取AttributeInfo的方法
	UFUNCTION(BlueprintCallable)
	FAuraAttributeInfo GetAttributeInfoByTag(const FGameplayTag& Tag, bool bLogError) const;
	
};
