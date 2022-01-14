// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBaseBullet.h"
#include "CUDamageBullet.generated.h"

UCLASS()
class CATCHUP_API ACUDamageBullet : public ACUBaseBullet
{
	GENERATED_BODY()

private:

	UPROPERTY()
	int32 Damage;

protected:
	
	ACUDamageBullet();

	virtual void ProcessHit(AActor* Target) override;
};
