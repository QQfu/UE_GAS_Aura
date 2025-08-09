// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
}

/**
 * 获取光标下的物体并执行高亮和取消高亮
 * 1. 获取光标碰撞结果
 * 2. 设置上一帧的碰撞物体和当前帧的碰撞物体
 * 3. 根据碰撞物体的结果设置是否执行高亮和取消高亮方法
 *	- 若LastActor和ThisActor都不为null，且相同，不做任何操作
 *	- 若LastActor和ThisActor都不为null，且不同，LastActor取消高亮，ThisActor高亮
 *	- 若LastActor或ThisActor为null，不做任何操作
 *	- 若LastActor不为null，LastActor取消高亮
 *	- 若ThisActor不为null，ThisActor高亮
 */
void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHitResult.GetActor());

	if (LastActor && LastActor != ThisActor)
	{
		LastActor->UnHighlightActor();
	}

	if (ThisActor && ThisActor != LastActor)
	{
		ThisActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::Printf(TEXT("Pressed: %s"), *InputTag.ToString()));

	if (GetAuraASC())
	{
		//把具体的处理逻辑交给AuraAbilitySystemComponent，实现解耦。
		GetAuraASC()->AbilityInputTagPressed(InputTag);
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Yellow, FString::Printf(TEXT("Released: %s"), *InputTag.ToString()));

	if (GetAuraASC())
	{
		//把具体的处理逻辑交给AuraAbilitySystemComponent，实现解耦。
		GetAuraASC()->AbilityInputTagReleased(InputTag);
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, FString::Printf(TEXT("Held: %s"), *InputTag.ToString()));

	if (GetAuraASC())
	{
		//把具体的处理逻辑交给AuraAbilitySystemComponent，实现解耦。
		GetAuraASC()->AbilityInputTagHeld(InputTag);
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraASC()
{
	if (AuraASC == nullptr)
	{
		AuraASC = Cast<UAuraAbilitySystemComponent>(	UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AuraASC;
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
	if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputSubsystem->AddMappingContext(AuraContext, 0);
	}

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
	UAuraEnhancedInputComponent* EnhancedInputComponent = CastChecked<UAuraEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(InputAction_Move, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	//绑定Ability Actions，对应按键，持续按键，释放按键
	EnhancedInputComponent->BindAbilityActions(InputDataAsset, this, &AAuraPlayerController::AbilityInputTagPressed, &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
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
