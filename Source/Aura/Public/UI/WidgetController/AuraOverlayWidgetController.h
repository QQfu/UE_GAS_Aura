// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraOverlayWidgetController.generated.h"

class UAuraUserWidget;
struct FGameplayTag;
struct FOnAttributeChangeData;
//动态多播代理，用于广播属性变化以及拱其他类绑定属性变化时的操作
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloatAttributeChangedSignature, float, NewValue);

//用于存储AssetTag对应的Message信息的Data Table Layout
USTRUCT(BlueprintType)
struct FAssetTagMessageRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

//用于将整个FAssetTagMessageRow广播出去
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTagToMessageWidgetSignature, FAssetTagMessageRow, Row);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitValues() override;

	virtual void BindCallbacksToDependencies() override;

	//Section start: 动态多播代理对应的变量
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnFloatAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnFloatAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnFloatAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnFloatAttributeChangedSignature OnMaxManaChanged;
	//Section end: 动态多播代理对应的变量

	UPROPERTY(BlueprintAssignable, Category="GAS|Message")
	FTagToMessageWidgetSignature TagToMessageWidgetDelegate;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> AssetTagMessageDataTable;

	//Section start: Attribute 数据变化后的处理函数
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
	//Section end: Attribute 数据变化后的处理函数

	template<typename T>
	T* FindDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

/**
 * 根据Tag查询Datatable的Row
 * @tparam T 泛型
 * @param DataTable 需要查询的DataTable
 * @param Tag 用于查询的Tag
 * @return Row
 */
template <typename T>
T* UAuraOverlayWidgetController::FindDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return  DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
