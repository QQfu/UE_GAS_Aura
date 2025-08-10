// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	/*
	 * top-down general settings
	 */
	//bOrientRotationToMovement: If true, rotate the Character toward the direction of acceleration, using RotationRate as the rate of rotation change.
	//Overrides UseControllerDesiredRotation. Normally you will want to make sure that other settings are cleared, such as bUseControllerRotationYaw on the Character.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//RotationRate: Change in rotation per second, used when UseControllerDesiredRotation or OrientRotationToMovement are true. Set a negative value for infinite rotation rate and instant turns.
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	//bConstrainToPlane: If true, movement will be constrained to a plane.
	GetCharacterMovement()->bConstrainToPlane = true;
	//bSnapToPlaneAtStart: If true and plane constraints are enabled, then the updated component will be snapped to the plane when first attached.
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init Ability Actor for Server
	InitAbilityActorInfo();

	//Init the Abilities
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	//Init Ability Actor for Client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetCharacterLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetCharacterLevel();
}

//返回武器插槽的位置
FVector AAuraCharacter::GetProjectileEmitLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(ProjectTileEmitSocketName);
}

/**
 * 1. 给AuraCharacter中的ASC和AS赋值
 * 2. 初始化 Ability Actor Info
 * 3. 初始化 OverlayWidget
 */
void AAuraCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	//初始化 Ability Actor Info
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

	//初始化绑定OnGameplayEffectAppliedDelegateToSelf
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	//初始化OverlayWidget
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlayWidget(AbilitySystemComponent, AttributeSet, AuraPlayerController, AuraPlayerState);
		}
	}

	//初始化Attribute
	InitAttributeFromGameplayEffect();
}
