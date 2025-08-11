// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	 * 初始化Components
	 * 1. 武器 Weapon
	 */
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "DefaultHandWeaponSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	//设置人物mesh和胶囊体的碰撞忽略摄像机
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::AddCharacterAbilities() const
{
	if (!HasAuthority()) return;

	if (UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->AddActorAbilities(StartupAbilities);
	}
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::InitAttributeFromGameplayEffect() const
{
	ApplyEffectToSelf(InitPrimaryAttributeEffect, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributeEffect, 1.f);
	ApplyEffectToSelf(InitVitalAttributeEffect, 1.f);
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const
{
	UAbilitySystemComponent* Asc = GetAbilitySystemComponent();
	check(Asc);
	check(EffectClass);

	FGameplayEffectContextHandle EffectContextHandle = Asc->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = Asc->MakeOutgoingSpec(EffectClass, Level, EffectContextHandle);
	Asc->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), Asc);
}
