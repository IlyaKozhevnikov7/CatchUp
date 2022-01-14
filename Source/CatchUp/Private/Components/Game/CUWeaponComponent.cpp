// Fill out your copyright notice in the Description page of Project Settings.

#include "CUWeaponComponent.h"

#include "CUCharacter.h"
#include "CUWeapon.h"

UCUWeaponComponent::UCUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Weapon = GetWorld()->SpawnActor<ACUWeapon>(WeaponClass);
	Weapon->AttachToComponent(GetOwner<ACUCharacter>()->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
}

void UCUWeaponComponent::Fire()
{
	if (IsActive())
	{
		Weapon->Fire();
	}
}

