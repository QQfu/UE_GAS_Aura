// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FAuraEffectProperties
{
	GENERATED_BODY()

	//Empty constructor
	FAuraEffectProperties() {}
	
	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};


/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	
	/* Start vital attribute definition */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health, Category= "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Mana, Category= "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	/* End vital attribute definition */

	/*Start primary attribute definition*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Strength, Category= "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Dexterity, Category= "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Dexterity);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Intelligence, Category= "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Vigor, Category= "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Spirit, Category= "Primary Attributes")
	FGameplayAttributeData Spirit;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Spirit);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const;
	/*End primary attribute definition*/

	/*Start Secondary Attribute Definition*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxHealth, Category= "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxMana, Category= "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_HealthRegeneration, Category= "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ManaRegeneration, Category= "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MinAttack, Category= "Secondary Attributes")
	FGameplayAttributeData MinAttack;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MinAttack);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxAttack, Category= "Secondary Attributes")
	FGameplayAttributeData MaxAttack;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxAttack);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_AbilityPower, Category= "Secondary Attributes")
	FGameplayAttributeData AbilityPower;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, AbilityPower);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CriticalHitChance, Category= "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CriticalHitBonus, Category= "Secondary Attributes")
	FGameplayAttributeData CriticalHitBonus;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitBonus);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Amor, Category= "Secondary Attributes")
	FGameplayAttributeData Amor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Amor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_AmorPenetration, Category= "Secondary Attributes")
	FGameplayAttributeData AmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, AmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Resistance, Category= "Secondary Attributes")
	FGameplayAttributeData Resistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ResistancePenetration, Category= "Secondary Attributes")
	FGameplayAttributeData ResistancePenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ResistancePenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_BlockChance, Category= "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_BlockBonus, Category= "Secondary Attributes")
	FGameplayAttributeData BlockBonus;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockBonus);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CriticalHitResistance, Category= "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_MinAttack(const FGameplayAttributeData& OldMinAttack) const;

	UFUNCTION()
	void OnRep_MaxAttack(const FGameplayAttributeData& OldMaxAttack) const;

	UFUNCTION()
	void OnRep_AbilityPower(const FGameplayAttributeData& OldAbilityPower) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitBonus(const FGameplayAttributeData& OldCriticalHitBonus) const;

	UFUNCTION()
	void OnRep_Amor(const FGameplayAttributeData& OldAmor) const;

	UFUNCTION()
	void OnRep_AmorPenetration(const FGameplayAttributeData& OldAmorPenetration) const;

	UFUNCTION()
	void OnRep_Resistance(const FGameplayAttributeData& OldResistance) const;
	
	UFUNCTION()
	void OnRep_ResistancePenetration(const FGameplayAttributeData& OldResistancePenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_BlockBonus(const FGameplayAttributeData& OldBlockBonus) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	
	/*End Secondary Attribute Definition*/

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FAuraEffectProperties& Props) const;
};
