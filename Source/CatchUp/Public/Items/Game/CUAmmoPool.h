// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/Actor.h"
#include "CUAmmoPool.generated.h"

UCLASS()
class CATCHUP_API ACUAmmoPool : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1"))
	int32 StartObjectAmount;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<ACUBaseBullet>> BulletsType;

	TMap<TSubclassOf<ACUBaseBullet>, TQueue<ACUBaseBullet*>> BulletPools;
	
protected:

	ACUAmmoPool();
	
	virtual void BeginPlay() override;

public:

	ACUBaseBullet* GetBullet(TSubclassOf<ACUBaseBullet> Type);
	
private:

	void InitPool();

	ACUBaseBullet* CreateBullet(TSubclassOf<ACUBaseBullet> Type);
	
	void OnBulletHited(ACUBaseBullet* Bullet);
	
};
