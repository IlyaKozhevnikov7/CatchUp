// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/Actor.h"
#include "CUWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FActiveChanged, const bool&);

class ACUPlayerController;
class ACUCharacter;
class ACUBaseBullet;
class ACUAmmoPool;

UCLASS()
class CATCHUP_API ACUWeapon : public AActor
{
	GENERATED_BODY()

public:

	FActiveChanged ActiveChangedEvent;
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	TArray<FBulletSet> Bullets;

	UPROPERTY()
	ACUAmmoPool* AmmoPool;

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACUBaseBullet> TempBulletType;
 	
protected:
	
	ACUWeapon();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:

	bool CanFire() const;
	
	void SetActive(const bool& bNewActive);

	void Fire(const FVector_NetQuantize& TargetLocation);

private:

	void InitAmmoPool();

	UFUNCTION()
	void OnRep_IsActive();
	
};
