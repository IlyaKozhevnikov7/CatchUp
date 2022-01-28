// Fill out your copyright notice in the Description page of Project Settings.

#include "CUHealthComponent.h"
#include "CatchUp/CatchUpMacros.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(CULogHealthComponent, All, All);

UCUHealthComponent::UCUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);

	MaxHealth = 100.f;
}

void UCUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	LastHealthAmount = CurrentHealth;
}

void UCUHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCUHealthComponent, CurrentHealth);
}

void UCUHealthComponent::Reset(const EGameRole& NewRole)
{
	check(COMPONENT_HAS_AUTHORITY);

	CurrentHealth = NewRole == EGameRole::Runner
		                ? MaxHealth
		                : 0.f;
}

void UCUHealthComponent::TakeDamage(const float& Amount)
{
	check(COMPONENT_HAS_AUTHORITY);
	check(Amount > 0.f);
	
	SetHealth(CurrentHealth - Amount);
}

void UCUHealthComponent::Heal(const float& Amount)
{
	check(COMPONENT_HAS_AUTHORITY);
	check(Amount > 0.f);

	SetHealth(CurrentHealth + Amount);
}

void UCUHealthComponent::SetHealth(const float& NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	
	if (NewHealth == 0)
		HealthOverEvent.Broadcast();
}

void UCUHealthComponent::OnRep_CurrentHealth()
{	
	if (CurrentHealth < LastHealthAmount)
		DamagedEvent.Broadcast(CurrentHealth, MaxHealth);
	
	if (CurrentHealth > LastHealthAmount)
		HealedEvent.Broadcast(CurrentHealth, MaxHealth);

	LastHealthAmount = CurrentHealth;
}
