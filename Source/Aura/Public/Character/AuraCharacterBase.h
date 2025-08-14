// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterBase();

	/*
	 * Getters for GAS
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	//Grant Startup Abilities
	void AddCharacterAbilities() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//武器
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	//用于蓝图端设置投射物发射位置的插槽名称
	UPROPERTY(EditAnywhere, Category="Combat")
	FName ProjectTileEmitSocketName;

	/*
	 * GAS
	 * 1. AbilitySystemComponent
	 * 2. AttributeSet
	 */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/**
	 * 作为父类方法拱子类重写
	 */
	virtual void InitAbilityActorInfo();

	/*
	 * 增加GameplayEffect类用于初始化Primary Attributes, Secondary Attributes, Vital Attributes
	 */
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Attribute|Primary Attribute")
	TSubclassOf<UGameplayEffect> InitPrimaryAttributeEffect;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Attribute|Secondary Attribute")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributeEffect;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category="Attribute|Vital Attribute")
	TSubclassOf<UGameplayEffect> InitVitalAttributeEffect;

	/*
	 * 用于初始化attribute
	 */
	void InitAttributeFromGameplayEffect() const;
	virtual void InitAttributeFromCharacterClassInfo() const;
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;

	//用于存放初始化的技能
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
