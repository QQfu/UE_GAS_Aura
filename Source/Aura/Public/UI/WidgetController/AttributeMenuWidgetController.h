// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FAuraAttributeInfo;
class UAuraAttributeInfoAsset;
/**
 * 
 */

//设置动态多播代理用于广播Attribute Info
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChangeInfoSignature, const FAuraAttributeInfo&, AttributeInfo);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitValues() override;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangeInfoSignature AttributeChangeInfoDelegate;

protected:
	//设置一个变量用于保存DataAsset Attribute info
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Attributes")
	TObjectPtr<UAuraAttributeInfoAsset> AttributeInfoAsset;
};
