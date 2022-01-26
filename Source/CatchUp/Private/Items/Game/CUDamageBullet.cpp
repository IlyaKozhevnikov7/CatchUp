// Fill out your copyright notice in the Description page of Project Settings.

#include "CUDamageBullet.h"
#include "CUHealthComponent.h"

ACUDamageBullet::ACUDamageBullet()
{
	Damage = 10.f;
}

void ACUDamageBullet::ProcessHit(AActor* Target)
{	
	if (HasAuthority() && Target)
	{
		if (auto HealthComponent = Target->FindComponentByClass<UCUHealthComponent>())
		{
			HealthComponent->TakeDamage(Damage);
		}
	}

	Super::ProcessHit(Target);
}
