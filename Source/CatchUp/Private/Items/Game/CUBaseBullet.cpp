// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBaseBullet.h"
#include "CUCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACUBaseBullet::ACUBaseBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	SetReplicatingMovement(true);

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(Collision);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
	Movement->bAutoActivate = false;
}

void ACUBaseBullet::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		OnActorHit.AddDynamic(this, &ACUBaseBullet::OnHit);
	}
}

void ACUBaseBullet::ProcessHit_Implementation(AActor* Target)
{
	if (HasAuthority())
	{
		Movement->Deactivate();
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		Mesh->SetVisibility(false);
		// проиграть fx
	}
}

void ACUBaseBullet::Activate()
{
	if (HasAuthority())
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Movement->Activate(true);
	}
	else
	{
		Mesh->SetVisibility(true);
	}
}

void ACUBaseBullet::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	check(HasAuthority());

	if (const auto Target = Cast<ACUCharacter>(OtherActor))
	{
		ProcessHit(Target);
		HitEvent.Broadcast(this); // для AmmoPool
	}
}

