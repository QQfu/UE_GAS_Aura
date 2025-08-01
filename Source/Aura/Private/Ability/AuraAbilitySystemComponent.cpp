// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::EffectAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Blue, FString("Effect Applied"));

	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTagsDelegate.Broadcast(TagContainer);
	
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectAppliedToSelf);
}
