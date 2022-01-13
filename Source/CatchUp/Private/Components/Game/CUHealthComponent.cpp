// Fill out your copyright notice in the Description page of Project Settings.

#include "CUHealthComponent.h"

#include "CatchUpTypes.h"
#include "CatchUp/CatchUpMacros.h"

UCUHealthComponent::UCUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	MaxHealth = 100.f;
}

void UCUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UCUHealthComponent::TakeDamage(const FDamage& Damage)
{
	check(COMPONENT_HAS_AUTHORITY);
	check(Damage.Amount > 0);

	SetHealth(FMath::Clamp(CurrentHealth - Damage.Amount, 0, MaxHealth));
	DamagedEvent.Broadcast();
}

void UCUHealthComponent::Heal(const int32& HealAmount)
{
	check(COMPONENT_HAS_AUTHORITY);
	check(HealAmount > 0);

	SetHealth(FMath::Clamp(CurrentHealth + HealAmount, 0, MaxHealth));
	HealedEvent.Broadcast();
}

void UCUHealthComponent::SetHealth(const int32& NewHealth)
{
	check(NewHealth >= 0 && NewHealth >= MaxHealth);
	CurrentHealth = NewHealth;
	
	if (NewHealth == 0)
		HealthOverEvent.Broadcast();
}
