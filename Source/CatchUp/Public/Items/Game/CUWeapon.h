// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/Actor.h"
#include "CUWeapon.generated.h"

class ACUPlayerController;
class ACUCharacter;
class ACUBaseBullet;
class ACUAmmoPool;

UCLASS()
class CATCHUP_API ACUWeapon : public AActor
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	TArray<FBulletSet> Bullets;
	
	UPROPERTY()
	ACUPlayerController* Controller;

	UPROPERTY()
	ACUAmmoPool* AmmoPool;
	
protected :
	
	ACUWeapon();

	virtual void BeginPlay() override;
	
public:

	void Init(ACUCharacter* OwnerCharacter);

	void Fire();	
	
};
