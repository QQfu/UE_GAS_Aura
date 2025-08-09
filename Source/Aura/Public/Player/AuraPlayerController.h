// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraInputDataAsset.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
struct FInputActionValue;
class IEnemyInterface;
class UInputAction;
class UInputMappingContext;
class UAuraAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	//定义InputActionConfigDataAsset用于指定输入按键的绑定
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UAuraInputDataAsset* InputDataAsset;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InputAction_Move;

	void Move(const FInputActionValue& InputActionValue);

	//用于识别mouse cursor hover
	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	//创建三个函数用于绑定按键，持续按键，释放按键的操作
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	//AuraAbilitySystemComponent变量和Get方法
	UAuraAbilitySystemComponent* AuraASC;
	UAuraAbilitySystemComponent* GetAuraASC();

	/**
	 * 角色移动所需要的变量
	* a.缓存的目标坐标：FVector CachedDestination
	* b.持续按键的时间：float FollowTime
	* c.判定为短按的时间：float ShortPressThreshold
	* d.判定为需要自动寻路的标记：bool bIsAutoRunning
	* e.判定需要停止寻路的距离：float AutoRunAcceptanceRadius
	* f.样条线：USplineComponent Spline
	* g.当前鼠标悬浮位置是否有敌人：bIsTargeting
	 */
	FVector CachedDestination = FVector::Zero();
	float FollowTime = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float ShortPressThreshold = 0.2f;
	bool bIsAutoRunning = false;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AutoRunAcceptanceRadius = 50.f;
	UPROPERTY(VisibleAnywhere)
	USplineComponent* Spline;
	bool bIsTargeting = false;

	//计算AutoRunning的方法
	void AutoRun();

	//定义一个类变量用于存放每帧获取的鼠标位置
	FHitResult CursorHitResult;
};
