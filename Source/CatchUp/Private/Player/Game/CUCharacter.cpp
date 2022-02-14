// Fill out your copyright notice in the Description page of Project Settings.

#include "CUCharacter.h"
#include "CUCharacterMovementComponent.h"
#include "CURunnerComponent.h"
#include "CUPlayerState.h"
#include "CUWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "CUSkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(CULogCharacter, All, All);

ACUCharacter::ACUCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer
	.SetDefaultSubobjectClass<UCUSkeletalMeshComponent>(ACharacter::MeshComponentName)
	.SetDefaultSubobjectClass<UCUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = true;
	AutoPossessAI = EAutoPossessAI::Disabled;

	bIsSliding = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	
	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Hands");
	HandsMesh->SetupAttachment(Camera);
	HandsMesh->bOnlyOwnerSee = true;
	HandsMesh->CastShadow = false;
	HandsMesh->SetVisibility(false);
	
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->CastShadow = true;
	
	RunnerComponent = CreateDefaultSubobject<UCURunnerComponent>("HealthComponent");
	RunnerComponent->bAutoActivate = false;
	
	WeaponComponent = CreateDefaultSubobject<UCUWeaponComponent>("WeaponComponent");
	WeaponComponent->bAutoActivate = false;
}

void ACUCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ACUCharacter, bIsSliding, COND_SimulatedOnly);
}

void ACUCharacter::PostActorCreated()
{
	Super::PostActorCreated();

	RoleMesh = Cast<UCUSkeletalMeshComponent>(GetMesh());
	Movement = Cast<UCUCharacterMovementComponent>(GetMovementComponent());
	check(RoleMesh);
}

void ACUCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		GetCharacterMovement()->bWantsToCrouch = true;
	}
	
	if (const auto CUPlayerState = Cast<ACUPlayerState>(NewController->PlayerState))
	{
		CUPlayerState->GameRoleChangedEvent.AddUObject(this, &ACUCharacter::OnGameRoleChanged);
	}
}

void ACUCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Реализовано здесь а не в OnRep_Controller тк Controller реплицируется только на владельца
	if (auto CUPlayerState = GetPlayerState<ACUPlayerState>())
	{
		CUPlayerState->GameRoleChangedEvent.AddUObject(this, &ACUCharacter::OnGameRoleChanged);
		// TODO сделать отписку от предыдущего PlayerState
	}
}

void ACUCharacter::ResetState()
{
	check(HasAuthority());
	
	const auto PlayerController = GetController<APlayerController>();
	check(PlayerController);

	const auto PlayrState = PlayerController->GetPlayerState<ACUPlayerState>();
	check(PlayrState);
	
	RunnerComponent->Reset();
	WeaponComponent->Reset(PlayerController);
	RoleMesh->SetRoleMesh(PlayrState->GetGameRole());
}

void ACUCharacter::OnDeactivated()
{
	
}

void ACUCharacter::OnGameRoleChanged(const EGameRole& NewRole)
{
	RoleMesh->SetRoleMesh(NewRole);
	
	if (HasAuthority())
	{		
		if (NewRole == EGameRole::Catcher)
		{
			WeaponComponent->Activate(true);
		}
		else
		{
			WeaponComponent->Deactivate();
			RunnerComponent->Reset();
		}
	}
	else
	{
		if (IsLocallyControlled())
		{
			HandsMesh->SetVisibility(NewRole == EGameRole::Catcher);
		}
	}
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
	InputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UCUWeaponComponent::Fire);
	InputComponent->BindAction("StopFire", IE_Released, WeaponComponent, &UCUWeaponComponent::StopFire);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &ACUCharacter::StartCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &ACUCharacter::StopCrouch);

	InputComponent->BindAction("Slide", IE_Pressed, Movement, &UCUCharacterMovementComponent::StartSlide);
	InputComponent->BindAction("Slide", IE_Released, Movement, &UCUCharacterMovementComponent::StopSlide);
	
	InputComponent->BindAxis("MoveForward", this, &ACUCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACUCharacter::MoveRight);
}

void ACUCharacter::MoveForward(float Amount)
{
	if (bIsSliding == false)
	{
		AddMovementInput(GetActorForwardVector() * Amount);
	}
}

void ACUCharacter::MoveRight(float Amount)
{
	if (bIsSliding == false)
	{
		AddMovementInput(GetActorRightVector() * Amount);
	}
}

void ACUCharacter::StartCrouch()
{
	GetCharacterMovement()->bWantsToCrouch = true;	
}

void ACUCharacter::StopCrouch()
{
	GetCharacterMovement()->bWantsToCrouch = false;
}
