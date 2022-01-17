// Fill out your copyright notice in the Description page of Project Settings.

#include "CUCharacter.h"
#include "CUHealthComponent.h"
#include "CUWeaponComponent.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(CULogCharacter, All, All);

ACUCharacter::ACUCharacter()
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
	
	HealthComponent = CreateDefaultSubobject<UCUHealthComponent>("HealthComponent");
	HealthComponent->bAutoActivate = false;
	
	WeaponComponent = CreateDefaultSubobject<UCUWeaponComponent>("WeaponComponent");
	WeaponComponent->bAutoActivate = false;

	GetMesh()->bOwnerNoSee = true;
	GetMesh()->CastShadow = true;
}

void ACUCharacter::SetupDefaultState()
{
	// выставить компонентам состояние по умолчанию
}

void ACUCharacter::OnActivated()
{
	UE_LOG(CULogCharacter, Display, TEXT("OnActivated"));
	
	SetupDefaultState();
}

void ACUCharacter::OnDeactivated()
{
	
}

void ACUCharacter::SetRoleColor()
{

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
