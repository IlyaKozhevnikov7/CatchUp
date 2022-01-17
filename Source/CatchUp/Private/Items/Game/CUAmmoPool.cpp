// Fill out your copyright notice in the Description page of Project Settings.

#include "CUAmmoPool.h"
#include "CUBaseBullet.h"

ACUAmmoPool::ACUAmmoPool()
{
	PrimaryActorTick.bCanEverTick = false;

	StartObjectAmount = 150;
}

void ACUAmmoPool::BeginPlay()
{
	Super::BeginPlay();

	InitPool();
}

ACUBaseBullet* ACUAmmoPool::GetBullet(const TSubclassOf<ACUBaseBullet>& Type)
{
	if (BulletPools[Type].IsEmpty())
		return CreateBullet(Type);

	ACUBaseBullet* Bullet = nullptr; 
	BulletPools[Type].Dequeue(Bullet);
	return Bullet;
}

void ACUAmmoPool::InitPool()
{
	for (auto Type : BulletsType)
	{
		for (int32 i = 0; i < StartObjectAmount; i++)
		{	
			BulletPools[Type].Enqueue(CreateBullet(Type));
		}		
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
	BulletPools[Bullet->GetClass()].Enqueue(Bullet);
}
