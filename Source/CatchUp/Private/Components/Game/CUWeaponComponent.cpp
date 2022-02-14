// Fill out your copyright notice in the Description page of Project Settings.

#include "CUWeaponComponent.h"
#include "CURunnerComponent.h"
#include "CatchUpMacros.h"
#include "CUBaseBullet.h"
#include "CUCharacter.h"
#include "CUSkeletalMeshComponent.h"
#include "CUWeapon.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

UCUWeaponComponent::UCUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
	bAutoActivate = false;

	ActiveChangedEvent.AddUObject(this, &UCUWeaponComponent::OnActiveChanged);
}

void UCUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (COMPONENT_HAS_AUTHORITY)
	{
		//check(WeaponClass);

		Weapon = GetWorld()->SpawnActor<ACUWeapon>(WeaponClass);

		const auto Mesh = Cast<UCUSkeletalMeshComponent>(GetOwner<ACharacter>()->GetMesh());
		check(Mesh);

		Weapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		
		OwnerHealth = GetOwner()->FindComponentByClass<UCURunnerComponent>();
		check(OwnerHealth);
	}
}

void UCUWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCUWeaponComponent, Weapon);
}

void UCUWeaponComponent::OnRep_IsActive()
{
	Super::OnRep_IsActive();

	ActiveChangedEvent.Broadcast(IsActive());
}

void UCUWeaponComponent::Reset(APlayerController* NewOwner)
{
	check(COMPONENT_HAS_AUTHORITY);

	Owner = NewOwner;
}

void UCUWeaponComponent::Fire()
{
	if (CanFire())
	{
		FireServer();
	}
}

void UCUWeaponComponent::StopFire()
{
	
}

bool UCUWeaponComponent::CanFire() const
{
	return IsActive() && Weapon->CanFire();
}

void UCUWeaponComponent::Activate(bool bReset)
{
	check(COMPONENT_HAS_AUTHORITY)
	
	Super::Activate(bReset);

	ActiveChangedEvent.Broadcast(IsActive());
}

void UCUWeaponComponent::Deactivate()
{
	check(COMPONENT_HAS_AUTHORITY)
	
	Super::Deactivate();
	
	ActiveChangedEvent.Broadcast(IsActive());
}

void UCUWeaponComponent::OnActiveChanged(const bool& bNewActive)
{
	if (COMPONENT_HAS_AUTHORITY)
	{
		Weapon->SetActive(bNewActive);
	}
	else
	{		
		if (GetOwner<APawn>()->IsLocallyControlled())
		{
			const auto Mesh = bNewActive ? GetOwner<ACUCharacter>()->GetHandsMesh() : GetOwner<ACUCharacter>()->GetMesh();
			Weapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");	
		}
	}
}

void UCUWeaponComponent::FireServer_Implementation()
{
#define LINE_TRACE_LENGTH 3000.f
	
	FVector Start;
	FRotator Direction;
	
	Owner->GetPlayerViewPoint(Start, Direction);
	FVector End = Start + (Direction.Vector() * LINE_TRACE_LENGTH);
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(Weapon);
	
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	const FVector Target = Hit.bBlockingHit ? Hit.ImpactPoint : End;
	DrawDebugBox(GetWorld(), Target, FVector(5.f), FColor::Magenta, false, 5.f, 0, 5.f);
	
	FBulletOwnerData InstigatorData;
	InstigatorData.Instigator = Owner;
	InstigatorData.OwnerHealth = OwnerHealth;
	
	Weapon->Fire(Target, InstigatorData);
}

bool UCUWeaponComponent::FireServer_Validate()
{
	return CanFire();
}

