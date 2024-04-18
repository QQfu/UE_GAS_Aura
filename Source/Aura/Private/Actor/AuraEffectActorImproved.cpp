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
	 */

	check(GameplayEffectClass);
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		FGameplayEffectContextHandle GameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		GameplayEffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle GameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.f, GameplayEffectContextHandle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
	}
}

// Called every frame
void AAuraEffectActorImproved::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

