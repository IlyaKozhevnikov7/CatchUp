// Fill out your copyright notice in the Description page of Project Settings.

#include "CUCharacter.h"
#include "CUHealthComponent.h"
#include "CUPlayerState.h"
#include "CUWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "CUSkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(CULogCharacter, All, All);

ACUCharacter::ACUCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCUSkeletalMeshComponent>(ACharacter::MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	
	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Hands");
	HandsMesh->SetupAttachment(Camera);
	HandsMesh->bOnlyOwnerSee = true;
	HandsMesh->CastShadow = false;

	GetMesh()->bOwnerNoSee = true;
	GetMesh()->CastShadow = true;
	
	HealthComponent = CreateDefaultSubobject<UCUHealthComponent>("HealthComponent");
	HealthComponent->bAutoActivate = false;
	
	WeaponComponent = CreateDefaultSubobject<UCUWeaponComponent>("WeaponComponent");
	WeaponComponent->bAutoActivate = false;
}

void ACUCharacter::PostActorCreated()
{
	Super::PostActorCreated();

	CUMesh = Cast<UCUSkeletalMeshComponent>(GetMesh());
}

void ACUCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (auto CUPlayerState = GetPlayerState<ACUPlayerState>())
	{
		CUPlayerState->GameRoleChangedEvent.AddUObject(this, &ACUCharacter::OnGameRoleChanged);
		// TODO сделать отписку от предыдущего PlayerState
	}
}

void ACUCharacter::SetupDefaultState()
{

}

// Вызывается только на сервере
void ACUCharacter::ResetState()
{
	
}

void ACUCharacter::OnDeactivated()
{
	
}

void ACUCharacter::OnGameRoleChanged(const EGameRole& NewRole)
{
	CUMesh->SetRoleMesh(NewRole);
}

float ACUCharacter::GetDirection() const
{
	if (GetVelocity().IsZero())
		return 0.f;
	
	const FVector NormalVelocity = GetVelocity().GetSafeNormal();
	
	const float Angle = FMath::Acos(
		FVector::DotProduct(GetActorForwardVector(), NormalVelocity));
	
	const float Sign = FMath::Sign(
		FVector::CrossProduct(GetActorForwardVector(), NormalVelocity).Z);

	return FMath::RadiansToDegrees(Angle * Sign);
}

void ACUCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	InputComponent->BindAxis("MoveForward", this, &ACUCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACUCharacter::MoveRight);
}

void ACUCharacter::MoveForward(float Amount)
{
	AddMovementInput(GetActorForwardVector() * Amount);
}

void ACUCharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector() * Amount);
}
