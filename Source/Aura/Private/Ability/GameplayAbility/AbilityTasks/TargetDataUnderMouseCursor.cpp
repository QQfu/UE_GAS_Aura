// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GameplayAbility/AbilityTasks/TargetDataUnderMouseCursor.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouseCursor* UTargetDataUnderMouseCursor::CreateTargetDataUnderMouseCursor(
	UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouseCursor* MyTask = NewAbilityTask<UTargetDataUnderMouseCursor>(OwningAbility);
	return MyTask;
}

void UTargetDataUnderMouseCursor::Activate()
{
	Super::Activate();

	if (const bool bIsLocalControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		//客户端逻辑
		/**
		 * 1. 获取鼠标坐标
		 * 2. 发送TargetData给服务器
		 * 3. 广播数据给客户端
		 */
		SendMouseCursorData();
	}
	else
	{
		//服务器端逻辑
		// 服务器必须等待客户端发送其目标数据。
		// 我们绑定到一旦数据到达就会广播的委托上。
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		// 绑定到目标数据就绪时触发的委托
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouseCursor::OnTargetDataReplicatedCallback);
		// 绑定到目标选择被取消时触发的委托
		AbilitySystemComponent.Get()->AbilityTargetDataCancelledDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouseCursor::OnTargetDataCancelledCallback);

		if (!AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey))
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouseCursor::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data,
	FGameplayTag ActivationTag)
{
	// 数据已成功从客户端复制过来。我们需要清楚服务器端的缓存数据
	AbilitySystemComponent.Get()->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	// 我们需要广播它，以便服务器上的技能蓝图图表可以继续执行。
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTargetDataReady.Broadcast(Data);
	}

	// 这个任务已经完成了它的使命。
	if (!bIsFinished)
	{
		bIsFinished = true;
		EndTask();
	}
}

void UTargetDataUnderMouseCursor::OnTargetDataCancelledCallback()
{
	// 客户端取消了目标选择。
	// 无论如何，任务都应该结束。
	if (!bIsFinished)
	{
		bIsFinished = true;
		EndTask();
	}
}

void UTargetDataUnderMouseCursor::OnDestroy(bool bInOwnerFinished)
{
	const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
	const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
	AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).RemoveAll(this);
	AbilitySystemComponent.Get()->AbilityTargetDataCancelledDelegate(SpecHandle, ActivationPredictionKey).RemoveAll(this);
	Super::OnDestroy(bInOwnerFinished);
}

void UTargetDataUnderMouseCursor::SendMouseCursorData()
{
	// 正确的获取PlayerController的方式：通过AbilityActorInfo
	// AbilityActorInfo缓存了所有与该技能相关的核心Actor，这是最直接和健壮的方法。
	const APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	
	//如果不存在合法的PlayerController，直接返回失败。
	if (!PlayerController)
	{
		EndTask();
		return;
	}

	FHitResult CursorHitResult;
	//获取鼠标碰撞结果，如果存在则返回成功和坐标信息；如果失败，则返回失败。
	if (const bool bSuccess = PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult))
	{
		//生成PredictionWindow
		FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());

		// 成功命中，创建 FGameplayAbilityTargetData_SingleTargetHit 实例
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
		TargetData->HitResult = CursorHitResult;

		// 创建一个 FGameplayAbilityTargetDataHandle 容器
		FGameplayAbilityTargetDataHandle TargetDataHandle;
        
		// 将我们创建的 TargetData 实例添加到容器中
		// FGameplayAbilityTargetDataHandle 的 Add 函数会接管 TargetData 的内存管理
		TargetDataHandle.Add(TargetData);

		//向服务器发送TargetData
		AbilitySystemComponent->ServerSetReplicatedTargetData(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey(),
			TargetDataHandle,
			FGameplayTag(),
			AbilitySystemComponent->ScopedPredictionKey);

		//判定是否需要广播
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			// 广播包含命中数据的委托
			OnTargetDataReady.Broadcast(TargetDataHandle);
		}
	}

	//结束任务，不能漏掉！
	if (!bIsFinished)
	{
		bIsFinished = true;
		EndTask();
	}
	
}
