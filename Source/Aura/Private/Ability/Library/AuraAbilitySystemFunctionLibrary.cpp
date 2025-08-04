// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Library/AuraAbilitySystemFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UAuraOverlayWidgetController* UAuraAbilitySystemFunctionLibrary::GetAuraOverlayWidgetController(
	const UObject* WorldContextObject)
{
	UAuraOverlayWidgetController* Result = nullptr;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FAuraWidgetControllerParams Params = FAuraWidgetControllerParams(ASC, AS, PC, PS);

			Result = HUD->GetAuraOverlayWidgetController(Params);
		}
	}

	return Result;
}

UAttributeMenuWidgetController* UAuraAbilitySystemFunctionLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	UAttributeMenuWidgetController* Result = nullptr;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FAuraWidgetControllerParams Params = FAuraWidgetControllerParams(ASC, AS, PC, PS);

			Result = HUD->GetAuraAttributeMenuWidgetController(Params);
		}
	}

	return Result;
}
