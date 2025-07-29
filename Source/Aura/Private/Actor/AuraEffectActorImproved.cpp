// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActorImproved.h"

#include "GameplayEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
AAuraEffectActorImproved::AAuraEffectActorImproved()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));

}

// Called when the game starts or when spawned
void AAuraEffectActorImproved::BeginPlay()
{
	Super::BeginPlay();
	
}

/**
 * 对TargetActor应用Gameplay Effect
 * @param TargetActor 目标Actor
 * @param GameplayEffectClass TSubclassOf<UGameplayEffect>
 */
void AAuraEffectActorImproved::ApplyEffectToTarget(AActor* TargetActor,
                                                   const TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	/*
	 * 1. Check GameplayEffectClass
	 * 2. 获取AbilitySystemComponent
	 * 3. 创建FGameplayEffectContextHandle
	 * 4. 配置GameplayEffectContext的Data
	 * 5. 创建FGameplayEffectSpecHandle
	 * 6. 使用ApplyGameplayEffectSpecToSelf将对应Data应用到自己身上
	 * 7. 如果GameplayEffectClass的类型是Infinite，且RemovalPolicy不是DoNotRemove，
	 * 则需要存储FActiveGameplayEffectHandle用以之后做Remove操作
	 */

	check(GameplayEffectClass);
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		FGameplayEffectContextHandle GameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		GameplayEffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle GameplayEffectSpecHandle =
			AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, EffectLevel, GameplayEffectContextHandle);
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());

		//存储FActiveGameplayEffectHandle
		const bool bIsInfinite = GameplayEffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

		if (bIsInfinite && InfiniteGameplayEffectRemovalPolicy != EEffectRemovalPolicy::DoNotRemove)
		{
			ActiveEffectHandlesMap.Add(ActiveGameplayEffectHandle, AbilitySystemComponent);
		}
	}
}

/**
 * 检查各种Effect的应用和移除策略并执行操作
 * 1. ApplyOnBeginOverlap：应用对应的Effect
 * @param Target 
 */
void AAuraEffectActorImproved::EffectOnBeginOverlap(AActor* Target)
{
	if (InstantGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(Target, InstantGameplayEffectClass);
	}

	if (DurationGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(Target, DurationGameplayEffectClass);
	}

	if (InfiniteGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(Target, InfiniteGameplayEffectClass);
	}
}

/**
* 检查各种Effect的应用和移除策略并执行操作
 * 1. ApplyOnEndOverlap：应用对应的Effect
 * @param Target 
 */
void AAuraEffectActorImproved::EffectOnEndOverlap(AActor* Target)
{
	if (InstantGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(Target, InstantGameplayEffectClass);
	}

	if (DurationGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(Target, DurationGameplayEffectClass);
	}

	if (InfiniteGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(Target, InfiniteGameplayEffectClass);
	}

	/*
	 * 1. 获取TargetASC
	 * 2. 遍历Map寻找对应的active handle
	 * 3. 把找到的Effect移除
	 * 4. 把找到的记录从Map中移除
	 */
	if (InfiniteGameplayEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);

		TArray<FActiveGameplayEffectHandle> HandlesToBeRemoved;
		for (auto EffectHandle : ActiveEffectHandlesMap)
		{
			if (EffectHandle.Key.IsValid() && EffectHandle.Value == TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffect(EffectHandle.Key, 1);
				HandlesToBeRemoved.Add(EffectHandle.Key);
			}
		}

		for (FActiveGameplayEffectHandle ToBeRemoved : HandlesToBeRemoved)
		{
			ActiveEffectHandlesMap.FindAndRemoveChecked(ToBeRemoved);
		}
	}
}

// Called every frame
void AAuraEffectActorImproved::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

