// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
};
