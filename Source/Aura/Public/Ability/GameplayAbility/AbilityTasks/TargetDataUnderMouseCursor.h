// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouseCursor.generated.h"

//定义一个Delegate用于广播鼠标Actor的位置
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataUnderMouseCursorSignature, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouseCursor : public UAbilityTask
{
	GENERATED_BODY()

public:
	//定义一个工厂函数用于创建自己的实例
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Target Data Under Mouse Cursor", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UTargetDataUnderMouseCursor* CreateTargetDataUnderMouseCursor(UGameplayAbility* OwningAbility);

	//Delegate变量
	UPROPERTY(BlueprintAssignable)
	FTargetDataUnderMouseCursorSignature  OnTargetDataReady;

protected:
	//重写Activate函数
	virtual void Activate() override;

	// 当目标数据从客户端复制过来时，在服务器上调用的回调函数
	UFUNCTION()
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);

	// 当客户端取消目标选择时，在服务器上调用的回调函数
	UFUNCTION()
	void OnTargetDataCancelledCallback();

	// 当任务被销毁时，用于清理绑定的委托
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	//用于发送客户端的鼠标坐标数据
	void SendMouseCursorData();
	
};
