// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
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
