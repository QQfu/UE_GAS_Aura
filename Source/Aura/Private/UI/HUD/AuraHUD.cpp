// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"


UAuraOverlayWidgetController* AAuraHUD::GetAuraOverlayWidgetController(const FAuraWidgetControllerParams& AwcParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UAuraOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(AwcParams);

		//绑定 Attribute 变化后的操作函数
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

/**
 * 1. 初始化 OverlayWidget
 * 2. 初始化 OverlayWidgetController
 * 3. 绑定 OverlayWidget 和 OverlayWidgetController
 * 4. 将 OverlayWidget 输出到 Viewport 中
 * 
 * @param ASC Input AbilitySystemComponent
 * @param AS Input AttributeSet
 * @param PC Input PlayerController
 * @param PS Input PlayerState
 */
void AAuraHUD::InitOverlayWidget(UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerController* PC,
                                 APlayerState* PS)
{
	checkf(OverlayWidgetClass, TEXT("The OverlayWidgetClass not set, please check the BP_AuraHUD."));
	checkf(OverlayWidgetControllerClass, TEXT("The OverlayWidgetControllerClass not set, please check the BP_AuraHUD."));
	
	UUserWidget* UserWidget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(UserWidget);

	const FAuraWidgetControllerParams AuraWidgetControllerParams(ASC, AS, PC, PS);

	UAuraOverlayWidgetController* WidgetController = GetAuraOverlayWidgetController(AuraWidgetControllerParams);
	OverlayWidget->SetWidgetController(WidgetController);

	//广播初始属性值
	WidgetController->BroadcastInitValues();

	OverlayWidget->AddToViewport();
}
