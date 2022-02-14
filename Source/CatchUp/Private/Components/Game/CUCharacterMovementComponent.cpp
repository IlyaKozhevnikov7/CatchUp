// Fill out your copyright notice in the Description page of Project Settings.

#include "CUCharacterMovementComponent.h"

#include "CatchUpMacros.h"
#include "CUCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

/*======================= FNetworkPredictionData_Client_CUMovement =======================*/

FNetworkPredictionData_Client_CUMovement::FNetworkPredictionData_Client_CUMovement(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr FNetworkPredictionData_Client_CUMovement::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_CUCharacter());
}

/*======================= FSavedMove_CUCharacter =======================*/

void FSavedMove_CUCharacter::Clear()
{
	Super::Clear();
	
	bSlidePressed = false;
	SlideDirection = FVector::ZeroVector;
}

uint8 FSavedMove_CUCharacter::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSlidePressed)
	{
		// FLAG_Custom_0 SlidePressed
		Result |= FLAG_Custom_0;
	}

	return Result;
}

void FSavedMove_CUCharacter::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UCUCharacterMovementComponent* Movement = Cast<UCUCharacterMovementComponent>(Character->GetCharacterMovement());
	check(Movement);

	SlideDirection = Movement->SlideDirection;
	bSlidePressed = Movement->bSlidePressed;
}

void FSavedMove_CUCharacter::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UCUCharacterMovementComponent* Movement = Cast<UCUCharacterMovementComponent>(Character->GetCharacterMovement());
	check(Movement);

	Movement->SlideDirection = SlideDirection;
	Movement->bSlidePressed = bSlidePressed;
}

/*======================= UCUCharacterMovementComponent =======================*/

UCUCharacterMovementComponent::UCUCharacterMovementComponent()
{
	SlideDirection = FVector::ZeroVector;
	CurrentSlideTime = 0.f;
	bSlidePressed = false;
}

void UCUCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	CUCharacter = GetOwner<ACUCharacter>();
}

FNetworkPredictionData_Client* UCUCharacterMovementComponent::GetPredictionData_Client() const
{	
	if (ClientPredictionData == nullptr)
	{
		UCUCharacterMovementComponent* MutableThis = const_cast<UCUCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_CUMovement(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UCUCharacterMovementComponent::PerformMovement(float DeltaTime)
{
	if (bSlidePressed && CanSlide())
	{
		SlideDirection = CharacterOwner->GetActorForwardVector();
		CurrentSlideTime = 1.2f; // sliding time (sec)
		CUCharacter->bIsSliding = true;
	}

	if (bSlidePressed && CUCharacter->bIsSliding)
	{
		const FVector CurrentLocation = CharacterOwner->GetActorLocation();
		const FVector DeltaLocation = (CurrentLocation + SlideDirection * 25.f) - CurrentLocation;
		
		MoveUpdatedComponent(DeltaLocation, SlideDirection.Rotation(), true);
		CurrentSlideTime -= DeltaTime;

		if (CurrentSlideTime <= 0.f)
		{
			StopSlide();
		}
	}

	Super::PerformMovement(DeltaTime);
}

void UCUCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	
	bSlidePressed = ((Flags & FSavedMove_CUCharacter::FLAG_Custom_0) != 0);

	if (bSlidePressed)
		StartSlide();
	else
		StopSlide();
}

bool UCUCharacterMovementComponent::CanSlide() const
{
	return IsMovingOnGround() && Velocity.Size() > 200.f;
}

void UCUCharacterMovementComponent::StartSlide()
{
	bSlidePressed = true;
}

void UCUCharacterMovementComponent::StopSlide()
{
	bSlidePressed = false;
	GetOwner<ACUCharacter>()->bIsSliding = false;
}

