// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AuraCharacterClassInfoAsset.generated.h"

class UGameplayAbility;
class UGameplayEffect;
//定义一个枚举存放Character Class的类型枚举
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Warrior,
	Ranger,
	Elementalist
};

//定义一个struct，用于存放Character Class对应的属性初始化信息
USTRUCT(BlueprintType)
struct FCharacterClassInfo
{
	GENERATED_BODY()

	//用于主属性初始化的GameplayEffect
	UPROPERTY(EditDefaultsOnly, Category = "Character Class|Attribute Default")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributeEffect;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraCharacterClassInfoAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	//定义一个ECharacterClass到FCharacterClassInfo的Map映射
	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TMap<ECharacterClass, FCharacterClassInfo> CharacterClassDefaultMap;

	//定义一个GameplayEffect用于初始化Secondary Attribute
	UPROPERTY(EditDefaultsOnly, Category = "Character Class|Attribute Common")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributeEffect;

	//定义一个GameplayEffect用于初始化Vital Attribute
	UPROPERTY(EditDefaultsOnly, Category = "Character Class|Attribute Common")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributeEffect;

	//定义Common Ability变量，用于蓝图端指定所有共有能力
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	//定义一个函数，用于根据ECharacterClass获取FCharacterClassInfo
	UFUNCTION(BlueprintCallable)
	FCharacterClassInfo GetCharacterClassInfo(ECharacterClass CharacterClass) const;
};
