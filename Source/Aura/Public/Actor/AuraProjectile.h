// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	//球形碰撞体
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	//定义球形碰撞体开始碰撞绑定的函数，必要的参数参考FComponentBeginOverlapSignature
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
