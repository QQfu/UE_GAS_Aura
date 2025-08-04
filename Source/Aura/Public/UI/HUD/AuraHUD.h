// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
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

	UFUNCTION(BlueprintCallable)
	UAuraOverlayWidgetController* GetAuraOverlayWidgetController(const FAuraWidgetControllerParams& AwcParams);

	UFUNCTION(BlueprintCallable)
	UAttributeMenuWidgetController* GetAuraAttributeMenuWidgetController(const FAuraWidgetControllerParams& AwcParams);

	//UFUNCTION(BlueprintCallable)
	void InitOverlayWidget(UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerController* PC, APlayerState* PS);

private:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAuraOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
};
