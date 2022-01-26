// Fill out your copyright notice in the Description page of Project Settings.

#include "CUAmmoPool.h"
#include "CUBaseBullet.h"

ACUAmmoPool::ACUAmmoPool()
{
	PrimaryActorTick.bCanEverTick = false;
	bNetLoadOnClient = false;
	
	StartObjectAmount = 150;
}

void ACUAmmoPool::BeginPlay()
{
	check(HasAuthority());
	
	Super::BeginPlay();

	InitPool();
}

ACUBaseBullet* ACUAmmoPool::GetBullet(const TSubclassOf<ACUBaseBullet>& Type)
{
	check(Pools.Contains(Type));

	const auto Bullet = *Pools[Type].FindByPredicate([](ACUBaseBullet* Element)
	{
		return Element->IsActive() == false;
	});

	if (Bullet == nullptr)
		return CreateBullet(Type);
	
	return Bullet;
}

void ACUAmmoPool::InitPool()
{
	for (auto Type : BulletsType)
	{
		Pools.Add(Type);
		
		for (int32 i = 0; i < StartObjectAmount; i++)
			Pools[Type].Add(CreateBullet(Type));
	}
}

ACUBaseBullet* ACUAmmoPool::CreateBullet(const TSubclassOf<ACUBaseBullet>& Type)
{
	const auto NewBullet = GetWorld()->SpawnActor<ACUBaseBullet>(Type);
	NewBullet->HitEvent.AddUObject(this, &ACUAmmoPool::OnBulletHited);
	return NewBullet;
}

void ACUAmmoPool::OnBulletHited(ACUBaseBullet* Bullet)
{
	Pools[Bullet->GetClass()].Add(Bullet);
}
