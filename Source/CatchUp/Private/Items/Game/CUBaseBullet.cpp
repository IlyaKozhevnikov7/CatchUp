// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBaseBullet.h"
#include "CUCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ACUBaseBullet::ACUBaseBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	SetReplicatingMovement(true);

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	Collision->SetGenerateOverlapEvents(false);
	Collision->BodyInstance.bNotifyRigidBodyCollision = true;
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Collision);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetGenerateOverlapEvents(false);
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

void ACUBaseBullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACUBaseBullet, bIsActive);
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
		Mesh->SetVisibility(false, true);
		// проиграть fx
	}
}

void ACUBaseBullet::Launch()
{
	if (HasAuthority())
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
		bIsActive = true;
		
		Movement->Velocity = GetActorForwardVector() * Movement->InitialSpeed;
		Movement->Activate(true);
	}
}

void ACUBaseBullet::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	check(HasAuthority());

	ProcessHit(OtherActor);
	HitEvent.Broadcast(this); // для AmmoPool
	
	bIsActive = false;
}

void ACUBaseBullet::OnRep_IsActive()
{
	Mesh->SetVisibility(bIsActive, true);
}

