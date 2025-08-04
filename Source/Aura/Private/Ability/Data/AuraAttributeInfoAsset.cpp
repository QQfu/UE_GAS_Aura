// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Data/AuraAttributeInfoAsset.h"

FAuraAttributeInfo UAuraAttributeInfoAsset::GetAttributeInfoByTag(const FGameplayTag& Tag, bool bLogError) const
{
	for (FAuraAttributeInfo Info : AttributeInformation)
	{
		if (Tag.MatchesTagExact(Info.AttributeTag)) return Info;
	}

	if (bLogError)
	{
		UE_LOG(LogTemp, Error, TEXT("Attribute not found. Tag: %s"), *Tag.ToString())
	}

	return FAuraAttributeInfo();
}
