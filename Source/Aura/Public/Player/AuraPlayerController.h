// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraInputDataAsset.h"
#include "AuraPlayerController.generated.h"

struct FInputActionValue;
class IEnemyInterface;
class UInputAction;
class UInputMappingContext;

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
};
