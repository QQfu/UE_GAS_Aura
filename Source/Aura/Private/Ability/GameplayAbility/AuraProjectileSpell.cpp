// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GameplayAbility/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//如果不是服务器端，则直接推出
	if (!HasAuthority(&ActivationInfo)) return;

	//利用SpawnActorDeferred创建Projectile
	/**
	 * 1. 定义FTransform参数
	 * 2. 定义FTransform中的location
	 * 3. 定义FTransform中的rotation
	 * 4. 调用SpawnActorDeferred
	 * 5. 设置Gameplay Effect
	 * 6. 调用FinishSpawning
	 */
	check(ProjectileClass);

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		FTransform Transform;
		//设置Location
		Transform.SetLocation(CombatInterface->GetProjectileEmitLocation());
		//TODO:设置Rotation

		//设置projectile
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			Transform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO:设置 Gameplay Effect

		//生成Projectile
		Projectile->FinishSpawning(Transform);
	}
}
