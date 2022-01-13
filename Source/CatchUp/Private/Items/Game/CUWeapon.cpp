// Fill out your copyright notice in the Description page of Project Settings.

#include "CUWeapon.h"
#include "CUCharacter.h"
#include "CUPlayerController.h"

ACUWeapon::ACUWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
}

void ACUWeapon::Init(ACUCharacter* OwnerCharacter)
{
	Controller = OwnerCharacter->GetController<ACUPlayerController>();
	check(Controller);
}

void ACUWeapon::Fire()
{
}
