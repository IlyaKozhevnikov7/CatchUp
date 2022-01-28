// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CUWeaponComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FActiveChanged, const bool&);

class ACUWeapon;
class UCUHealthComponent;

UCLASS(ClassGroup = (CatchUp), meta = (BlueprintSpawnableComponent))
class CATCHUP_API UCUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	FActiveChanged ActiveChangedEvent;
	
private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACUWeapon> WeaponClass;

	UPROPERTY()
	UCUHealthComponent* OwnerHealth;
	
	UPROPERTY()
	APlayerController* Owner;
	
	UPROPERTY(Replicated)
	ACUWeapon* Weapon;
	
protected:
	
	UCUWeaponComponent();
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void OnRep_IsActive() override;
	
public:

	void Reset(APlayerController* NewOwner);
	
	void Fire();

	void StopFire();
	
	FORCEINLINE bool CanFire() const;

	virtual void Activate(bool bReset) override;

	virtual void Deactivate() override;
	
private:

	UFUNCTION()
	void OnActiveChanged(const bool& bNewActive);

	UFUNCTION(Server, Reliable, WithValidation)
	void FireServer();
	
};
