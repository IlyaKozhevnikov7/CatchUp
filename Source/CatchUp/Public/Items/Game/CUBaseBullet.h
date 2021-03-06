// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CUBaseBullet.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FHit, class ACUBaseBullet*);

class ACUCharacter;
class USphereComponent;
class UProjectileMovementComponent;
class UCURunnerComponent;

USTRUCT()
struct FBulletOwnerData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	AController* Instigator;
	
	UPROPERTY()
	UCURunnerComponent* OwnerHealth;
	
};

UCLASS(Abstract)
class CATCHUP_API ACUBaseBullet : public AActor
{
	GENERATED_BODY()

public:

	FHit HitEvent;
	
private:

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* Collision;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* Movement;

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive;

protected:

	FBulletOwnerData InstigatorData;
	
protected:

	ACUBaseBullet();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		
	UFUNCTION(NetMulticast, Reliable)
	virtual void ProcessHit(AActor* Target);
	
public:

	FORCEINLINE bool IsActive() const { return bIsActive; }

	virtual void Init(FBulletOwnerData OwnerData) { }
	
	void Launch();
	
private:

	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnRep_IsActive();
	
};
