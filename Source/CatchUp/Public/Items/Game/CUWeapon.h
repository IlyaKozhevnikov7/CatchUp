// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/Actor.h"
#include "CUWeapon.generated.h"

class ACUPlayerController;

UCLASS()
class CATCHUP_API ACUWeapon : public AActor
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly)
	FHit HitInfo;

	UPROPERTY()
	ACUPlayerController* Controller;
	
protected:

	ACUWeapon();

public:

	void Init(ACUCharacter* Owner);

	void Fire();
	
};
