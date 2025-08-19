// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	//Override for init ability actor info
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Start Combat Interface*/
	virtual int32 GetCharacterLevel_Implementation() const override;

	//实现GetProjectileEmitLocation返回投射物的发射位置
	virtual FVector GetProjectileEmitLocation_Implementation() const override;
	
	/** End Combat Interface*/

private:
	virtual void InitAbilityActorInfo() override;
};
