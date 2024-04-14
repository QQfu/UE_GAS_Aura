// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
struct FAuraWidgetControllerParams;
class UAuraOverlayWidgetController;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UFUNCTION(BlueprintCallable)
	UAuraOverlayWidgetController* GetAuraOverlayWidgetController(const FAuraWidgetControllerParams& AwcParams);

	//UFUNCTION(BlueprintCallable)
	void InitOverlayWidget(UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerController* PC, APlayerState* PS);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAuraOverlayWidgetController> OverlayWidgetController;
};
