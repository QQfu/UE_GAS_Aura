// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AuraAbilitySystemGlobals.h"

#include "AuraGameplayEffectTypes.h"

FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAuraGameplayEffectContext();
}
