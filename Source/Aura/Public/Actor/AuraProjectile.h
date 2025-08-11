// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "AuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraProjectile();

	//投射物移动组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	//定义replicate策略
	bool bIsReplicate = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//设置生命周期
	UPROPERTY(EditDefaultsOnly, Category = "Life Span")
	float LifeSpan = 5.f;
	
	//球形碰撞体
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	//定义球形碰撞体开始碰撞绑定的函数，必要的参数参考FComponentBeginOverlapSignature
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//定义视效,Niagara
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraSystem> ProjectileHitEffect;
	
	//定义音效
	//定义撞击音效
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<USoundBase> ProjectileHitSound;

	//定义飞行音效
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<USoundBase> ProjectileFlySound;
	//定义一个AudioComponent用于在生命周期内播放FlySound
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UAudioComponent> ProjectileFlyAudioComponent;


	//重写Destroy函数，如果客户端提前接收到了Destroy的Replication，那么需要执行一次PerformSphereOnBeginOverlap方法后再Destroy。
	virtual void Destroy();


private:
	//用于指定碰撞逻辑
	void PerformSphereOnBeginOverlap();
	//用于客户端存放PerformSphereOnBeginOverlap是否已经被执行过了
	bool bHasPerformedSphereOnBeginOverlap = false;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
