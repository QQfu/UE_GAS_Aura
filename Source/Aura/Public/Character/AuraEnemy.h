// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Data/AuraCharacterClassInfoAsset.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
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
	virtual UAnimMontage* GetHitReactMontage_Implementation() const override;
	/**End Combat Interface*/

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	void BindHealthBarAttributeChangeDelegates();
	void BindTagEventDelegates();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Level = 1;

	float LifeSpan = 5.f;

	/*Start 定义动画资产*/
	//定义HitReact Montage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	/*End 定义动画资产*/

	//定义一个WidgetComponent用于使用HealthBar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> HealthBar;

	//定义两个delegate用于广播 Health change 和 MaxHealth change
	UPROPERTY(BlueprintAssignable)
	FOnFloatAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnFloatAttributeChangedSignature OnMaxHealthChanged;

	//定义一个变量指定Character Class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterClass CharacterClass;

	//重写方法进行属性初始化
	virtual void InitAttributeFromCharacterClassInfo() const override;

	//定义一个函数用于处理HitReact相关的逻辑
	UFUNCTION()
	void PerformHitReactByTagChange(const FGameplayTag Tag, int32 Count);

	//处理死亡逻辑
	virtual void PerformDie() override;
};
