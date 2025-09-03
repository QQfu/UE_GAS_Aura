// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GameplayAbility/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

/**
 * 利用SpawnActorDeferred创建Projectile
 * 1. 定义FTransform参数
 * 2. 定义FTransform中的location
 * 3. 定义FTransform中的rotation
 * 4. 调用SpawnActorDeferred
 * 5. 设置Gameplay Effect
 * 6. 调用FinishSpawning
 */
void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation) 
{
	//如果不是服务器端，则直接推出
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	
	check(ProjectileClass);

	if (const ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		FTransform Transform;
		//设置Location
		const FVector StartLocation = CombatInterface->GetProjectileEmitLocation_Implementation();
		Transform.SetLocation(StartLocation);
		//设置Rotation
		FRotator Rotation = (TargetLocation - StartLocation).Rotation();
		Rotation.Pitch = 0.f; 
		Transform.SetRotation(Rotation.Quaternion());

		//设置projectile
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			Transform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//设置 Damage Gameplay Effect
		/**
		 * 1. 获取ASC
		 * 2. 生成EffectContext
		 * 3. 生成EffectSpecHandle
		 * 4. 为Projectile的DmageEffectSpecHandle
		 */
		if (DamageEffect)
		{
			if (const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
			{
				FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
				Context.AddSourceObject(Projectile);
				Context.SetAbility(this);
				TArray<TWeakObjectPtr<AActor>> Actors;
				Actors.Add(Projectile);
				Context.AddActors(Actors);
				const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), Context);

				//为Damage标签设置Set By Caller Magnitude的值
				const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
				//从BaseDamage中取出对应的基础伤害值
				const float BaseDamageMagnitude = BaseDamage.GetValueAtLevel(GetAbilityLevel());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, BaseDamageMagnitude);
				Projectile->DamageEffectSpecHandle = SpecHandle;
			}
		}

		//生成Projectile
		Projectile->FinishSpawning(Transform);
	}
}
