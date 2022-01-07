// Fill out your copyright notice in the Description page of Project Settings.

#include "CUCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(CULogCharacter, All, All);

ACUCharacter::ACUCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	
	SetupDefaultState();
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

	InputComponent->BindAxis("MoveForward", this, &ACUCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACUCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookAt", this, &APawn::AddControllerPitchInput);
}

void ACUCharacter::MoveForward(float Amount)
{
	AddMovementInput(GetActorForwardVector() * Amount);
}

void ACUCharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector() * Amount);
}
