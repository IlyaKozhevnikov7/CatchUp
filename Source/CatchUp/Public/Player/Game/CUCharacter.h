// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/Character.h"
#include "CUCharacter.generated.h"

class UCameraComponent;
class UCUHealthComponent;
class UCUWeaponComponent;
class UCUSkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLanded, const FHitResult&, LandHit);

UCLASS()
class CATCHUP_API ACUCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FLanded LandedEvent;
	
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
	
	void OnActivated();
	
	void OnDeactivated();

	void SetRoleColor();
	
private:
	
	ACUCharacter(const FObjectInitializer& ObjectInitializer);

	void SetupDefaultState();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveForward(float Amount);
	
	void MoveRight(float Amount);

};
