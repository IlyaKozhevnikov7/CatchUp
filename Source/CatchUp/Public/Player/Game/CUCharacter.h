// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/Character.h"
#include "CUCharacter.generated.h"

class ACUPlayerState;
class UCameraComponent;
class UCUCharacterMovementComponent;
class UCURunnerComponent;
class UCUWeaponComponent;
class UCUSkeletalMeshComponent;

UCLASS()
class CATCHUP_API ACUCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bIsSliding;
	
protected:

  	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* HandsMesh;

	UPROPERTY(EditDefaultsOnly)
	UCUSkeletalMeshComponent* RoleMesh;

	UPROPERTY(EditDefaultsOnly)
	UCUCharacterMovementComponent* Movement;
	
	UPROPERTY(EditDefaultsOnly)
	UCURunnerComponent* RunnerComponent;

	UPROPERTY(EditDefaultsOnly)
	UCUWeaponComponent* WeaponComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<EGameRole, FColor> RoleColors;
	
public:

	FORCEINLINE USkeletalMeshComponent* GetHandsMesh() const { return HandsMesh; }
	
	void ResetState();
	
	void OnDeactivated();
	
	void OnGameRoleChanged(const EGameRole& NewRole);

	UFUNCTION(BlueprintPure)
	float GetDirection() const;
	
private:
	
	ACUCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PostActorCreated() override;

	virtual void PossessedBy(AController* NewController) override;
	
	void OnRep_PlayerState() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveForward(float Amount);
	
	void MoveRight(float Amount);

	void StartCrouch();

	void StopCrouch();
	
};
