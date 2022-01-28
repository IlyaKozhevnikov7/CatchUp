// Fill out your copyright notice in the Description page of Project Settings.

#include "CURunnerComponent.h"
#include "CatchUp/CatchUpMacros.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(CULogHealthComponent, All, All);

UCURunnerComponent::UCURunnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);

	MaxHealth = 100.f;
}

void UCURunnerComponent::BeginPlay()
{
	Super::BeginPlay();

	Reset();
}

void UCURunnerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCURunnerComponent, CurrentHealth);
}

void UCURunnerComponent::Reset()
{
	CurrentHealth = MaxHealth;
	LastHealthAmount = CurrentHealth;
}

void UCURunnerComponent::TakeDamage(const float& Damage, AController* Instigator)
{
	check(COMPONENT_HAS_AUTHORITY);
	check(Damage > 0.f);
	
	SetHealth(CurrentHealth - Damage);

	if (CurrentHealth == 0)
	{
		const auto Runner = GetOwner<APawn>()->GetController();
		check(Runner);
		
		CaughtEvent.Broadcast(Runner, Instigator);
	}
}

void UCURunnerComponent::Heal(const float& Amount)
{
	check(COMPONENT_HAS_AUTHORITY);
	check(Amount > 0.f);

	SetHealth(CurrentHealth + Amount);
}

void UCURunnerComponent::SetHealth(const float& NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
}

void UCURunnerComponent::OnRep_CurrentHealth()
{	
	if (CurrentHealth < LastHealthAmount)
		DamagedEvent.Broadcast(CurrentHealth, MaxHealth);
	else
		HealedEvent.Broadcast(CurrentHealth, MaxHealth);

	LastHealthAmount = CurrentHealth;
}
