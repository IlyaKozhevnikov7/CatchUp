// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CUCharacterMovementComponent.generated.h"

class ACUCharacter;

UENUM()
enum ECUMovementMode
{
	Sliding = 1
};
	
class CATCHUP_API FNetworkPredictionData_Client_CUMovement : public FNetworkPredictionData_Client_Character
{	
	typedef FNetworkPredictionData_Client_Character Super;

public:
	
	FNetworkPredictionData_Client_CUMovement(const UCharacterMovementComponent& ClientMovement);

	virtual FSavedMovePtr AllocateNewMove() override;
};

class CATCHUP_API FSavedMove_CUCharacter : public FSavedMove_Character 
{
	typedef FSavedMove_Character Super;

public:

	FVector SlideDirection;
	bool bSlidePressed;
	
	virtual void Clear() override;
	
	virtual uint8 GetCompressedFlags() const override;

	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;

	virtual void PrepMoveFor(class ACharacter* Character) override;
	
};

UCLASS()
class CATCHUP_API UCUCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	friend FSavedMove_CUCharacter;
	
private:

	UPROPERTY()	
	ACUCharacter* CUCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category = "Slide")
	UCurveFloat* SlidingSpeed;

	FVector SlideDirection;

	float CurrentSlideTime;
	
	bool bSlidePressed;
	
public:

	FORCEINLINE bool WantToSlide() const { return bSlidePressed; }
	
	void StartSlide();

	void StopSlide();

private:
	
	UCUCharacterMovementComponent();

	virtual void BeginPlay() override;
	
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	virtual void PerformMovement(float DeltaTime) override;
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	bool CanSlide() const;
	
};
