// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FAuraWidgetControllerParams
{
	GENERATED_BODY()

	FAuraWidgetControllerParams() {}

	FAuraWidgetControllerParams(UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet, APlayerController* InPlayerController, APlayerState* InPlayerState)
	: AbilitySystemComponent(InAbilitySystemComponent), AttributeSet(InAttributeSet), PlayerController(InPlayerController), PlayerState(InPlayerState) {}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FAuraWidgetControllerParams& WidgetControllerParams);

	//用于广播初始化属性
	virtual void BroadcastInitValues();

	//用于绑定 Attribute 变化时的处理函数
	virtual void BindCallbacksToDependencies();

protected:

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;
};
