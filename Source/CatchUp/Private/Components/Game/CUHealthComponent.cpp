// Fill out your copyright notice in the Description page of Project Settings.

#include "CUHealthComponent.h"
#include "CatchUp/CatchUpMacros.h"

DEFINE_LOG_CATEGORY_STATIC(CULogHealthComponent, All, All);

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
	check(Amount > 0.f);
	
	SetHealth(CurrentHealth - Amount);
	DamagedEvent.Broadcast();

	UE_LOG(CULogHealthComponent, Display, TEXT("%s Damaged [%f]"), *GetOwner()->GetName(), Amount);
}

void UCUHealthComponent::Heal(const float& Amount)
{
	check(COMPONENT_HAS_AUTHORITY);
	check(Amount > 0.f);

	SetHealth(CurrentHealth + Amount);
	HealedEvent.Broadcast();
}

void UCUHealthComponent::SetHealth(const float& NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	
	if (NewHealth == 0)
		HealthOverEvent.Broadcast();
}
