// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CUWeaponComponent.generated.h"

class ACUWeapon;

UCLASS(ClassGroup = (CatchUp), meta = (BlueprintSpawnableComponent))
class CATCHUP_API UCUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACUWeapon> WeaponClass;
	
	UPROPERTY()
	ACUWeapon* Weapon;

protected:
	
	UCUWeaponComponent();
	
	virtual void BeginPlay() override;

public:

	void Fire();
	
};
