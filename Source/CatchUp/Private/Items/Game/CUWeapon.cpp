// Fill out your copyright notice in the Description page of Project Settings.

#include "CUWeapon.h"
#include "CUCharacter.h"

ACUWeapon::ACUWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
}

void ACUWeapon::Init(ACUCharacter* Owner)
{
	Controller = Owner->GetController<ACUPlayerController>();
	check(Controller);
}

void ACUWeapon::Fire()
{
}
