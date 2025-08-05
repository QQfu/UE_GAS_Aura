// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AuraAttributeSet.h"

#include "AuraGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	//初始化属性映射
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	AttributeTagsToStaticFuncPtr.Add(AuraGameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	AttributeTagsToStaticFuncPtr.Add(AuraGameplayTags.Attributes_Primary_Dexterity, GetDexterityAttribute);
	AttributeTagsToStaticFuncPtr.Add(AuraGameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	AttributeTagsToStaticFuncPtr.Add(AuraGameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);
	AttributeTagsToStaticFuncPtr.Add(AuraGameplayTags.Attributes_Primary_Spirit, GetSpiritAttribute);
	AttributeTagsToStaticFuncPtr.Add(AuraGameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	AttributeTagsToStaticFuncPtr.Add(AuraGameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	//Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Spirit, COND_None, REPNOTIFY_Always);

	//Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MinAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, AbilityPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Amor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, AmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistancePenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MinAttack(const FGameplayAttributeData& OldMinAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MinAttack, OldMinAttack);
}

void UAuraAttributeSet::OnRep_MaxAttack(const FGameplayAttributeData& OldMaxAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxAttack, OldMaxAttack);
}

void UAuraAttributeSet::OnRep_AbilityPower(const FGameplayAttributeData& OldAbilityPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, AbilityPower, OldAbilityPower);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitBonus(const FGameplayAttributeData& OldCriticalHitBonus) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitBonus, OldCriticalHitBonus);
}

void UAuraAttributeSet::OnRep_Amor(const FGameplayAttributeData& OldAmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Amor, OldAmor);
}

void UAuraAttributeSet::OnRep_AmorPenetration(const FGameplayAttributeData& OldAmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, AmorPenetration, OldAmorPenetration);
}

void UAuraAttributeSet::OnRep_Resistance(const FGameplayAttributeData& OldResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resistance, OldResistance);
}

void UAuraAttributeSet::OnRep_ResistancePenetration(const FGameplayAttributeData& OldResistancePenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistancePenetration, OldResistancePenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_BlockBonus(const FGameplayAttributeData& OldBlockBonus) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockBonus, OldBlockBonus);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Dexterity, OldDexterity);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Spirit, OldSpirit);
}

/**
 * override 生命周期函数用于在属性发生变化前规定结果范围：不小于0且不大于Max值
 * @param Attribute 
 * @param NewValue 
 */
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

/**
 * 获取source和target的信息用于后置处理
 * @param Data 回调信息
 */
void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FAuraEffectProperties EffectProperties;

	SetEffectProperties(Data, EffectProperties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
	FAuraEffectProperties& Props) const
{
	//获取source的Actor，ASC，Controller，Character
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetInstigatorAbilitySystemComponent();
	const TSharedPtr<FGameplayAbilityActorInfo> SourceGameplayAbilityActorInfo =
		Props.SourceASC ? Props.SourceASC->AbilityActorInfo : nullptr;
	if (SourceGameplayAbilityActorInfo.IsValid())
	{
		Props.SourceAvatarActor = SourceGameplayAbilityActorInfo->AvatarActor.Get();
		Props.SourceController = SourceGameplayAbilityActorInfo->PlayerController.Get();
		Props.SourceCharacter = Props.SourceController ? Props.SourceController->GetCharacter() : nullptr;
	}


	//获取target的Actor，ASC，Controller，Character
	const TSharedPtr<FGameplayAbilityActorInfo> TargetGameplayAbilityActorInfo = Data.Target.AbilityActorInfo;
	if (TargetGameplayAbilityActorInfo.IsValid())
	{
		Props.TargetASC = TargetGameplayAbilityActorInfo->AbilitySystemComponent.Get();
		Props.TargetAvatarActor = TargetGameplayAbilityActorInfo->AvatarActor.Get();
		Props.TargetController = TargetGameplayAbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Props.TargetController ? Props.TargetController->GetCharacter() : nullptr;
	}
}
