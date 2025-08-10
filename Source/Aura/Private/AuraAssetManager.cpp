// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"  // 添加此行以包含UAbilitySystemGlobals

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	return *Cast<UAuraAssetManager>(GEngine->AssetManager);
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitAuraNativeGameplayTags();

	//如果需要使用TargetData，则需要下面这行代码
	UAbilitySystemGlobals::Get().InitGlobalData();
}
