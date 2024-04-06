// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*
	 * 绑定Input Mapping Context
	 * 1. 获取EnhancedInputLocalPlayerSubsystem
	 * 2. 绑定Input Mapping Context
	 * 3. 其他设置
	 */
	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(EnhancedInputSubsystem);
	EnhancedInputSubsystem->AddMappingContext(AuraContext, 0);

	//Whether the mouse cursor should be displayed.
	bShowMouseCursor = true;

	//Data structure used to setup an input mode that allows the UI to respond to user input, and if the UI doesn't handle it player input / player controller gets a chance.
	FInputModeGameAndUI InputModeData;
	//SetLockMouseToViewportBehavior: Sets the mouse locking behavior of the viewport
	//EMouseLockMode::DoNotLock: Do not lock the mouse cursor to the viewport
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//SetHideCursorDuringCapture: Whether to hide the cursor during temporary mouse capture caused by a mouse down
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}
