// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
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
	//设置与Projectile Overlap
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
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

void AAuraCharacterBase::InitAttributeFromCharacterClassInfo() const
{
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

void AAuraCharacterBase::PerformDie()
{
	/**
	 * 1. 卸载武器
	 * 2. 调用MulticastHandleDeath
	 */
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	PerformDissolve();
	MulticastHandleDeath();
}

void AAuraCharacterBase::PerformDissolve()
{
	/**
	 * 1. 生成动态材质
	 * 2. 为骨骼体和武器替换材质
	 * 3. 调用Timeline函数
	 */
	if (DissolveMaterialInstance && WeaponDissolveMaterialInstance)
	{
		UMaterialInstanceDynamic* InstanceDynamic = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		UMaterialInstanceDynamic* WeaponInstanceDynamic = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);

		GetMesh()->SetMaterial(0, InstanceDynamic);
		Weapon->SetMaterial(0, WeaponInstanceDynamic);

		DissolveTimeline(InstanceDynamic);
		WeaponDissolveTimeline(WeaponInstanceDynamic);
	}
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	/**
	 * 1. 设置武器碰撞
	 * 2. 设置骨骼碰撞
	 * 3. 设置胶囊体碰撞
	 */
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
