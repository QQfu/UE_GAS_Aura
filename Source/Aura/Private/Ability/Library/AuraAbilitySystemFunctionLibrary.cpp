// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Library/AuraAbilitySystemFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "Game/AuraGameModeBase.h"
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

void UAuraAbilitySystemFunctionLibrary::InitCharacterDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	/**
	 * 1. 获取GameMode并转化成AuraGameMode
	 * 2. 利用入参Character Class获取FCharacterClassInfo
	 * 3. 利用FCharacterClassInfo获取Primary、Secondary、Vital属性的GameplayEffect Class
	 * 4. 利用对应的GameplayEffect和入参ASC初始化Attribute
	 */
	if (const AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		if (const UAuraCharacterClassInfoAsset* ClassInfoAsset = GameMode->AuraCharacterClassInfoAsset)
		{
			FCharacterClassInfo ClassInfo = ClassInfoAsset->GetCharacterClassInfo(CharacterClass);

			if (ASC)
			{
				const AActor* AvatarActor = ASC->GetAvatarActor();

				//Primary
				FGameplayEffectContextHandle PrimaryHandle = ASC->MakeEffectContext();
				PrimaryHandle.AddSourceObject(AvatarActor);
				const FGameplayEffectSpecHandle PrimarySpecHandle = ASC->MakeOutgoingSpec(ClassInfo.DefaultPrimaryAttributeEffect, Level, PrimaryHandle);
				ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

				//Secondary
				FGameplayEffectContextHandle SecondaryHandle = ASC->MakeEffectContext();
				SecondaryHandle.AddSourceObject(AvatarActor);
				const FGameplayEffectSpecHandle SecondarySpecHandle = ASC->MakeOutgoingSpec(ClassInfoAsset->DefaultSecondaryAttributeEffect, Level, SecondaryHandle);
				ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

				//Vital
				FGameplayEffectContextHandle VitalHandle = ASC->MakeEffectContext();
				VitalHandle.AddSourceObject(AvatarActor);
				const FGameplayEffectSpecHandle VitalSpecHandle = ASC->MakeOutgoingSpec(ClassInfoAsset->DefaultVitalAttributeEffect, Level, VitalHandle);
				ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());

			}
		}
	}
}

void UAuraAbilitySystemFunctionLibrary::GiveCommonAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	if (const AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		if (const UAuraCharacterClassInfoAsset* ClassInfoAsset = GameMode->AuraCharacterClassInfoAsset)
		{
			TArray<TSubclassOf<UGameplayAbility>> CommonAbilities = ClassInfoAsset->CommonAbilities;

			if (ASC)
			{
				for (const TSubclassOf<UGameplayAbility> Ability : CommonAbilities)
				{
					FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability,1.f);
					ASC->GiveAbility(AbilitySpec);
				}
			}
		}
	}
}

UAuraCharacterClassInfoAsset* UAuraAbilitySystemFunctionLibrary::GetDamageCoefficients(const UObject* WorldContextObject)
{
	if (const AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return GameMode->AuraCharacterClassInfoAsset;
	}
	else
	{
		return nullptr;
	}
}
