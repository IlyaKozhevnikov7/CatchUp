// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/Character.h"
#include "CUCharacter.generated.h"

class ACUPlayerState;
class UCameraComponent;
class UCUHealthComponent;
class UCUWeaponComponent;
class UCUSkeletalMeshComponent;

UCLASS()
class CATCHUP_API ACUCharacter : public ACharacter
{
	GENERATED_BODY()
	
protected:

  	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* HandsMesh;

	UPROPERTY(EditDefaultsOnly)
	UCUSkeletalMeshComponent* CUMesh;
	
	UPROPERTY(EditDefaultsOnly)
	UCUHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly)
	UCUWeaponComponent* WeaponComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<EGameRole, FColor> RoleColors;
	
public:

	void ResetState();
	
	void OnDeactivated();
	
	void OnGameRoleChanged(const EGameRole& NewRole);

	UFUNCTION(BlueprintPure)
	float GetDirection() const;
	
private:

	ACUCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PostActorCreated() override;

	void OnRep_PlayerState() override;
	
	void SetupDefaultState();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveForward(float Amount);
	
	void MoveRight(float Amount);
	
};
