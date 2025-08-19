// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "Ability/AuraAttributeSet.h"
#include "Ability/Library/AuraAbilitySystemFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	/*
	 * Construct GAS
	 * 1. AbilitySystemComponent
	 * 2. AttributeSet
	 */
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	//初始化HealthBar
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetMesh());
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetCharacterLevel_Implementation() const
{
	return Level;
}

UAnimMontage* AAuraEnemy::GetHitReactMontage_Implementation() const
{
	return HitReactMontage;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	//为HealthBar绑定属性变化的回调函数
	BindHealthBarAttributeChangeDelegates();

	//绑定Tag Event
	BindTagEventDelegates();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	//Init ability actor info
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	//初始化绑定OnGameplayEffectAppliedDelegateToSelf
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	//Temp Work：利用AuraCharacter初始化属性的方式来初始化Enemy的属性
	//InitAttributeFromGameplayEffect();

	//根据Character Class来初始化属性
	InitAttributeFromCharacterClassInfo();

	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	//UE_LOG(LogTemp, Warning, TEXT("%s, %s, %f"), *GetName(), *AuraAttributeSet->GetStrengthAttribute().GetName(), AuraAttributeSet->GetStrength());

	//初始化Common Ability
	UAuraAbilitySystemFunctionLibrary::GiveCommonAbilities(this, AbilitySystemComponent);
}

void AAuraEnemy::BindHealthBarAttributeChangeDelegates()
{
	//将HealthBar的Widget Controller绑定为自己
	if (UAuraUserWidget* UserWidget = Cast<UAuraUserWidget>(HealthBar->GetWidget()))
	{
		UserWidget->SetWidgetController(this);
	}

	/**
	 * 1. 获取AttributeSet
	 * 2. 获取AbilitySystemComponent
	 * 3. 绑定AttributeChangeDelete
	 * 4. 广播Health和MaxHealth
	 */
	if (const UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		//初始化时，广播一次当前的属性值，必须在InitAbilityActorInfo之后调用
		OnHealthChanged.Broadcast(AS->GetHealth());
		OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	}
}

void AAuraEnemy::BindTagEventDelegates()
{
	//为Effects.HitReact绑定Tag Event对应的函数
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::PerformHitReactByTagChange);
}

void AAuraEnemy::InitAttributeFromCharacterClassInfo() const
{
	Super::InitAttributeFromCharacterClassInfo();

	/**
	 * 调用AuraAbilitySystemFunctionLibrary::InitCharacterDefaultAttributes进行初始化
	 */
	UAuraAbilitySystemFunctionLibrary::InitCharacterDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::PerformHitReactByTagChange(const FGameplayTag Tag, int32 Count)
{
}

void AAuraEnemy::PerformDie()
{

	//设置LifeSpan
	SetLifeSpan(LifeSpan);
	
	Super::PerformDie();
}
