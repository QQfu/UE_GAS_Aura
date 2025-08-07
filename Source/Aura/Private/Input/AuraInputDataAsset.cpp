// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputDataAsset.h"

const UInputAction* UAuraInputDataAsset::FindInputActionByTag(const FGameplayTag& InputTag, bool bLogError) const
{

	for (auto [InputAction, InputActionTag] : InputConfigAsset)
	{
		if (InputAction && InputActionTag.MatchesTagExact(InputTag))
		{
			return InputAction;
		}
	}

	if (bLogError)
	{
		UE_LOG(LogTemp, Error, TEXT("Input Action not found for tag: %s"), *InputTag.ToString());
	}

	return nullptr;
}
