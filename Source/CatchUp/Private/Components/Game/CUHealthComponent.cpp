// Fill out your copyright notice in the Description page of Project Settings.

#include "CUHealthComponent.h"
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

void UCUHealthComponent::TakeDamage(const float& Amount)
{
	check(COMPONENT_HAS_AUTHORITY);

	//SetHealth(FMath::Clamp(CurrentHealth - Damage.Amount, 0.f, MaxHealth));
	DamagedEvent.Broadcast();
}

void UCUHealthComponent::Heal(const float& Amount)
{
	check(COMPONENT_HAS_AUTHORITY);
	check(Amount > 0.f);

	SetHealth(FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth));
	HealedEvent.Broadcast();
}

void UCUHealthComponent::SetHealth(const float& NewHealth)
{
	CurrentHealth = NewHealth;
	
	if (NewHealth == 0)
		HealthOverEvent.Broadcast();
}
