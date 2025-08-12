// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public ICombatInterface
{
	GENERATED_BODY()

public:

	AAuraEnemy();

	/**Start Enemy Interface*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/**End Enemy Interface*/

	/**Start Combat Interface*/
	virtual int32 GetCharacterLevel_Implementation() const override;
	/**End Combat Interface*/

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	void BindHealthBarAttributeChangeDelegates();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Level = 1;

	//定义一个WidgetComponent用于使用HealthBar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> HealthBar;

	//定义两个delegate用于广播 Health change 和 MaxHealth change
	UPROPERTY(BlueprintAssignable)
	FOnFloatAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnFloatAttributeChangedSignature OnMaxHealthChanged;
};
