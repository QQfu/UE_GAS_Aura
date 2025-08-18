// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Data/AuraCharacterClassInfoAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "AuraAbilitySystemFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//用于获取OverlayWidgetController
	UFUNCTION(BlueprintPure, Category = "AbilitySystem|BlueprintFunctionLibrary")
	static UAuraOverlayWidgetController* GetAuraOverlayWidgetController(const UObject* WorldContextObject);

	//用于获取AttributeMenuWidgetController
	UFUNCTION(BlueprintPure, Category = "AbilitySystem|BlueprintFunctionLibrary")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	//定义静态方法用于初始化默认属性值
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|BlueprintFunctionLibrary")
	static void InitCharacterDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	//定义静态方法用于初始化Common Ability
	static void GiveCommonAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
};
