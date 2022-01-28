// Fill out your copyright notice in the Description page of Project Settings.

#include "CUDamageBullet.h"
#include "CURunnerComponent.h"

ACUDamageBullet::ACUDamageBullet()
{
	Damage = 10.f;
}

void ACUDamageBullet::ProcessHit(AActor* Target)
{	
	if (HasAuthority() && Target)
	{
		if (auto HealthComponent = Target->FindComponentByClass<UCURunnerComponent>())
		{
			HealthComponent->TakeDamage(Damage, InstigatorData.Instigator);
			InstigatorData.OwnerHealth->Heal(Damage);
		}
	}

	Super::ProcessHit(Target);
}

void ACUDamageBullet::Init(FBulletOwnerData OwnerData)
{
	InstigatorData = OwnerData;
}
