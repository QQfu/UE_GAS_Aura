// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"

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
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	//Init Ability Actor for Client
	InitAbilityActorInfo();
}

/**
 * 1. 给AuraCharacter中的ASC和AS赋值
 * 2. 初始化Ability Actor Info
 */
void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
}
