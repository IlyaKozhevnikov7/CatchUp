// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBaseBullet.h"
#include "CUCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACUBaseBullet::ACUBaseBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	SetReplicatingMovement(true);

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Collision);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
	Movement->ProjectileGravityScale = 0.f;
	Movement->InitialSpeed = 2000.f;
	Movement->bAutoActivate = false;
	
	bIsActive = false;
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

void ACUBaseBullet::Launch_Implementation(const FVector_NetQuantize& TargetLocation)
{
	if (HasAuthority())
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
		bIsActive = true;
		
		Movement->Velocity = GetActorForwardVector() * Movement->InitialSpeed;
		Movement->Activate(true);
	}
	else
	{
		Mesh->SetVisibility(true, true);
	}
}

void ACUBaseBullet::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	check(HasAuthority());

	if (const auto Target = Cast<ACUCharacter>(OtherActor))
	{
		ProcessHit(Target);
		HitEvent.Broadcast(this); // для AmmoPool
		
		bIsActive = false;
	}
}

