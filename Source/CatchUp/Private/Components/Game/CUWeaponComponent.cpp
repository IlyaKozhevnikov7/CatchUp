// Fill out your copyright notice in the Description page of Project Settings.

#include "CUWeaponComponent.h"
#include "CatchUpMacros.h"
#include "CUCharacter.h"
#include "CUWeapon.h"

UCUWeaponComponent::UCUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(WeaponClass) == true)
	{
		Weapon = GetWorld()->SpawnActor<ACUWeapon>(WeaponClass);
	}
	
	OnComponentActivated.AddDynamic(this, &UCUWeaponComponent::OnActivated);
}

void UCUWeaponComponent::Fire()
{
	if (IsActive())
	{
		Weapon->Fire();
	}
}

void UCUWeaponComponent::OnActivated(UActorComponent* Component, bool bReset)
{
	if (COMPONENT_HAS_AUTHORITY == false)
	{
		if (ensure(Weapon) == true)
		{
			Weapon->AttachToComponent(GetOwner<ACUCharacter>()->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		}
	}
}

