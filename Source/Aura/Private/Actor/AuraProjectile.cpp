// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"

// Sets default values
AAuraProjectile::AAuraProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//初始化球形碰撞体，并定义它的碰撞参数
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//初始化ProjectileMovementComponent，并配置参数
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	//设置生命周期
	SetLifeSpan(LifeSpan);

	//绑定球形碰撞体开始碰撞的处理函数
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereBeginOverlap);

	//利用ProjectileFlySound 初始化UAudioComponent ProjectileFlyAudioComponent
	if (ProjectileFlySound)
	{
		ProjectileFlyAudioComponent = UGameplayStatics::SpawnSoundAttached(ProjectileFlySound, RootComponent);
	}
}

void AAuraProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PerformSphereOnBeginOverlap();

	if (HasAuthority())
	{
		//服务器端，Destroy自身
		Destroy();
	}
	else
	{
		//客户端，将bHasPerformedSphereOnBeginOverlap设置为true
		bHasPerformedSphereOnBeginOverlap = true;
	}
}

void AAuraProjectile::Destroy()
{
	if (!HasAuthority() && !bHasPerformedSphereOnBeginOverlap)
	{
		PerformSphereOnBeginOverlap();
	}

	Super::Destroy();
}

/**
 * 1. 生成撞击视效
 * 2. 播放撞击音效
 */
void AAuraProjectile::PerformSphereOnBeginOverlap() 
{
	//生成Niagara视效
	if (ProjectileHitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ProjectileHitEffect, GetActorLocation());
	}

	// 2. 播放撞击音效
	if (ProjectileHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileHitSound, GetActorLocation());
	}

	//停止 ProjectileFlyAudioComponent
	if (ProjectileFlyAudioComponent)
	{
		ProjectileFlyAudioComponent->Stop();
	}
}

// Called every frame
void AAuraProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

