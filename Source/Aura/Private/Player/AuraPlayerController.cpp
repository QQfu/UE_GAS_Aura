// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/*
	 * 绑定Enhanced Input Actions
	 */
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(InputAction_Move, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

/*
 * 1. 找到当前Controller的前方和右方向量
 * 2. 在Controlled Pawn中利用AddMovementInput方法进行移动
 */
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	//1.找到当前Controller的前方和右方向量
	const FRotator ControllerRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControllerRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//2.在Controlled Pawn中利用AddMovementInput方法进行移动
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
