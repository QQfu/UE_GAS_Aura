// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::Instance;

void FAuraGameplayTags::InitAuraNativeGameplayTags()
{
	//初始化Primary Attribute Tags
	Instance.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("The strength increases the attack power, amor and attack penetration."));
	Instance.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Dexterity"), FString("The dexterity increases the critical hit chance and bonus."));
	Instance.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("The intelligence increases the ability power, resistance and penetration."));
	Instance.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), FString("The vigor increases the max health, health regeneration and block."));
	Instance.Attributes_Primary_Spirit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Spirit"), FString("The spirit increases the max mana, mana regeneration and critical resistance."));
	
	//初始化Secondary Attribute Tags
	Instance.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("The health cannot exceed the max health."));
	Instance.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("The mana cannot exceed the max mana."));
	Instance.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("The health regenerated per second."));
	Instance.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("The mana regenerated per second."));
	Instance.Attributes_Secondary_MinAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MinAttack"), FString("The min physical damage per hit."));
	Instance.Attributes_Secondary_MaxAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxAttack"), FString("The max physical damage per hit."));
	Instance.Attributes_Secondary_AbilityPower = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.AbilityPower"), FString("The ability damage increases this value."));
	Instance.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("The chance to perform a critical hit."));
	Instance.Attributes_Secondary_CriticalHitBonus = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitBonus"), FString("The damage coefficient when perform a critical hit."));
	Instance.Attributes_Secondary_Amor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Amor"), FString("Decreases the amount of physical damage."));
	Instance.Attributes_Secondary_AmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.AmorPenetration"), FString("Ignore the amor from target."));
	Instance.Attributes_Secondary_Resistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Resistance"), FString("Decreases the amount of ability damage."));
	Instance.Attributes_Secondary_ResistancePenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ResistancePenetration"), FString("Ignore the resistance from target."));
	Instance.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("Chance to block a hit."));
	Instance.Attributes_Secondary_BlockBonus = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockBonus"), FString("Reduces the damage when block a hit."));
	Instance.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("Decreases the chance of critical hit when get damage."));

	//初始化Input Tags
	Instance.Input_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.LMB"), FString("Input for left mouse button."));
	Instance.Input_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.RMB"), FString("Input for right mouse button."));
	Instance.Input_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.1"), FString("Input for keyboard 1."));
	Instance.Input_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.2"), FString("Input for keyboard 2."));
	Instance.Input_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.3"), FString("Input for keyboard 3."));
	Instance.Input_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.4"), FString("Input for keyboard 4."));
	Instance.Input_C = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.C"), FString("Input for keyboard C."));

	//初始化Set By Caller Tags
	Instance.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage Set By Caller Magnitude Tag."));

	//初始化 Tags for Tag Events
	Instance.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Tag used to trigger Hit Reacting Event."));
}
