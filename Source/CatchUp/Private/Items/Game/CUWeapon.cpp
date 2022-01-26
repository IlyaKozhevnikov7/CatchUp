// Fill out your copyright notice in the Description page of Project Settings.

#include "CUWeapon.h"
#include "CUCharacter.h"
#include "CUAmmoPool.h"
#include "CUDamageBullet.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

ACUWeapon::ACUWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SetReplicates(true);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetVisibility(false, true);
}

void ACUWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		InitAmmoPool();
		check(AmmoPool);
	}
	
	check(Mesh);
}

void ACUWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACUWeapon, bIsActive);
}

bool ACUWeapon::CanFire() const
{
	return bIsActive;
}

void ACUWeapon::SetActive(const bool& bNewActive)
{
	check(HasAuthority());
	
	bIsActive = bNewActive;
}

void ACUWeapon::Fire(const FVector_NetQuantize& TargetLocation)
{
	check(HasAuthority());
	
	const auto Bullet = AmmoPool->GetBullet(TempBulletType);
	check(Bullet);

	const FVector MuzzleLocation = Mesh->GetSocketLocation("Muzzle");
	
	Bullet->SetActorLocationAndRotation(MuzzleLocation, (TargetLocation - MuzzleLocation).Rotation());
	Bullet->Launch(TargetLocation);
}

void ACUWeapon::InitAmmoPool()
{
	for (const auto Pool : TActorRange<ACUAmmoPool>(GetWorld()))
	{
		AmmoPool = Pool;
		break;
	}
}

void ACUWeapon::OnRep_IsActive()
{
	//Mesh->SetVisibility(bIsActive, true);
	
	ActiveChangedEvent.Broadcast(bIsActive);
}
