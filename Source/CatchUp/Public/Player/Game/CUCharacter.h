// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/Character.h"
#include "CUCharacter.generated.h"

class UCUHealthComponent;
class UCUWeaponComponent;

UCLASS()
class CATCHUP_API ACUCharacter : public ACharacter
{
	GENERATED_BODY()

private:

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
	
	ACUCharacter();

	void SetupDefaultState();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveForward(float Amount);
	
	void MoveRight(float Amount);

};
