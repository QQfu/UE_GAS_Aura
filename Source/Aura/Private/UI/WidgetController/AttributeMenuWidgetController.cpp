// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "Ability/AuraAttributeSet.h"
#include "Ability/Data/AuraAttributeInfoAsset.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	if (const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		for (auto Pair : AuraAttributeSet->AttributeTagsToStaticFuncPtr)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
				[this, Pair, AuraAttributeSet] (const FOnAttributeChangeData& Data)
				{
					FAuraAttributeInfo Info = AttributeInfoAsset->GetAttributeInfoByTag(Pair.Key);
					Info.AttributeValue = Pair.Value().GetNumericValue(AuraAttributeSet);
					AttributeChangeInfoDelegate.Broadcast(Info);
				}
			);
		}
	}
}

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	Super::BroadcastInitValues();

	if (const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		for (auto Pair : AuraAttributeSet->AttributeTagsToStaticFuncPtr)
		{
			FAuraAttributeInfo Info = AttributeInfoAsset->GetAttributeInfoByTag(Pair.Key);
			Info.AttributeValue = Pair.Value().GetNumericValue(AuraAttributeSet);
			AttributeChangeInfoDelegate.Broadcast(Info);
		}
	}
}
