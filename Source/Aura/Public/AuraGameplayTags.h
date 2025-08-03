// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Singleton
 */
struct FAuraGameplayTags
{
public:
	//单例类的Get方法
	static FAuraGameplayTags Get() { return Instance;}

	//初始化Tags的方法
	static void InitAuraNativeGameplayTags();

	//定义关联Native Tag的变量
	//Primary Attribute Tags
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Spirit;

	//Secondary Attribute Tags
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MinAttack;
	FGameplayTag Attributes_Secondary_MaxAttack;
	FGameplayTag Attributes_Secondary_AbilityPower;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitBonus;
	FGameplayTag Attributes_Secondary_Amor;
	FGameplayTag Attributes_Secondary_AmorPenetration;
	FGameplayTag Attributes_Secondary_Resistance;
	FGameplayTag Attributes_Secondary_ResistancePenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_BlockBonus;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;

protected:

private:

	static FAuraGameplayTags Instance;
};
