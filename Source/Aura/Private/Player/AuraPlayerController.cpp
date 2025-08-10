// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	//初始化Spline
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();

	AutoRun();
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
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHitResult.GetActor());

	if (LastActor == ThisActor) return; // No change, do nothing
	
	// Unhighlight the previous actor if it exists and is different from the current one
	if (LastActor) LastActor->UnHighlightActor(); // Always unhighlight if LastActor exists
	
	// Highlight the current actor if it exists and is different from the previous one
	if (ThisActor) ThisActor->HighlightActor();
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::Printf(TEXT("Pressed: %s"), *InputTag.ToString()));

	if (GetAuraASC())
	{
		//把具体的处理逻辑交给AuraAbilitySystemComponent，实现解耦。
		GetAuraASC()->AbilityInputTagPressed(InputTag);
	}

	/**
	 * Movement判断：
	 * 1. 如果ThisActor不是nullptr，则bIsTargeting设置为true
	 * 2. 如果是LMB按下，则将bIsAutoRunning设置为false
	 */
	bIsTargeting = ThisActor ? true : false;
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		// 只有在瞄准敌人时，才用鼠标左键打断自动寻路。
		// 如果是点击地面，我们的意图是开始一段新的寻路，不应在此处打断。
		if (bIsTargeting)
		{
			bIsAutoRunning = false;
		}
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Yellow, FString::Printf(TEXT("Released: %s"), *InputTag.ToString()));

	/**
	* i.判断InputTag是不是LMB
	*	1.如果是，继续判断bIsTargeting
	*		a.如果不是，处理Movement
	*			i.判断是短按，且ControlledPawn存在
	*			ii.调用UNavigationSystemV1的FindPathToLocationSynchronously找到至目标位置的路径
	*				1.需要在地图中放置NavVolume
	*				2.需要在项目中包含NavigationSystem
	*			iii.将路径点加入到Spline中
	*			iv.增加一条Debug指令，将所有的路径点都画出来
	*	2.如果是，调用AuraAbilitySystemComponent的Released方法
	*ii.重置FolowTime和bIsTargeting
	*/
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		if (bIsTargeting)
		{
			if (GetAuraASC())
			{
				//把具体的处理逻辑交给AuraAbilitySystemComponent，实现解耦。
				GetAuraASC()->AbilityInputTagReleased(InputTag);
			}
		}
		else
		{
			if (const APawn* ControlledPawn = GetPawn(); ControlledPawn && FollowTime < ShortPressThreshold)
			{
				// 寻路并检查路径是否有效且包含至少一个点
				if (UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination); NavigationPath && NavigationPath->PathPoints.Num() > 0)
				{
					// 使用 .Last() 是获取最后一个元素最安全、最清晰的方式
					CachedDestination = NavigationPath->PathPoints.Last();
					
					Spline->ClearSplinePoints();
					// 使用 const& 避免在循环中不必要的拷贝，是更好的C++实践
					for (const FVector& PathPoint : NavigationPath->PathPoints)
					{
						Spline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);
						// DrawDebugSphere(GetWorld(), PathPoint, 8.f, 16, FColor::Red, false, 3.f);
					}

					bIsAutoRunning = true;
				}
			}
		}
	}
	FollowTime = 0.f;
	bIsTargeting = false;
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, FString::Printf(TEXT("Held: %s"), *InputTag.ToString()));

	/**
	* i.判断InputTag是不是LMB
	*	1.如果是，继续判断bIsTargeting
	*		a.如果是，激活Ability（调用AuraAbilitySystemComponent的方法）
	*		b.如果不是，处理Movement
	*			i.增加FollowTime
	*			ii.获取鼠标HitResult
	*			iii.设置CachedDestination = 鼠标位置
	*			iv.设置Direction = CachedDestination - 角色位置 的标准化向量（AddMovementInput）
	* ii.如果不是，激活Ability（调用AuraAbilitySystemComponent的方法）
	*/
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		if (bIsTargeting)
		{
			if (GetAuraASC())
			{
				//把具体的处理逻辑交给AuraAbilitySystemComponent，实现解耦。
				GetAuraASC()->AbilityInputTagHeld(InputTag);
			}
		}
		else
		{
			FollowTime += GetWorld()->GetDeltaSeconds();

			if (CursorHitResult.bBlockingHit)
			{
				CachedDestination = CursorHitResult.Location;
				if (FollowTime >= ShortPressThreshold)
				{
					bIsAutoRunning = false;
					const FVector Direction = (CachedDestination - GetPawn()->GetActorLocation()).GetSafeNormal();
					GetPawn()->AddMovementInput(Direction);
				}
			}
		}
	}
	else
	{
		if (GetAuraASC())
		{
			//把具体的处理逻辑交给AuraAbilitySystemComponent，实现解耦。
			GetAuraASC()->AbilityInputTagHeld(InputTag);
		}
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


/**
* i.获取Spline中离角色最近的点和朝向
* ii.将朝向设置到Movement中（AddMovementInput）
* iii.计算CachedDestination和角色位置的距离
* iv.如果距离小于AutoRunAcceptanceRadius，将bIsAutoRunning设置为false
 */
void AAuraPlayerController::AutoRun()
{
	if (!bIsAutoRunning)
	{
		return;
	}
	
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector SplineLocation = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector SplineDirection = Spline->FindDirectionClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(SplineDirection);

		if (const float Distance = (SplineLocation - CachedDestination).Length(); Distance < AutoRunAcceptanceRadius)
		{
			bIsAutoRunning = false;
		}
	}
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

	//Data structure used to set up an input mode that allows the UI to respond to user input, and if the UI doesn't handle it player input / player controller gets a chance.
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

	// 如果玩家有任何手动移动输入，则取消自动寻路
	if (InputAxisVector.IsNearlyZero() == false)
	{
		bIsAutoRunning = false;
	}

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
