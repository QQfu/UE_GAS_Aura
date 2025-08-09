// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AuraAbilitySystemComponent.h"

#include "Ability/GameplayAbility/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AddActorAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities)
{
	for (const TSubclassOf<UGameplayAbility> Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability,1.f);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				AbilitySpecInputPressed(AbilitySpec);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				AbilitySpecInputReleased(AbilitySpec);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				AbilitySpecInputPressed(AbilitySpec);
				if (!AbilitySpec.IsActive())
				{
					TryActivateAbility(AbilitySpec.Handle);
				}
			}
		}
	}
}

void UAuraAbilitySystemComponent::EffectAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent,
                                                      const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) const
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
