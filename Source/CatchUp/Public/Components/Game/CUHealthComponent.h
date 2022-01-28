// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "Components/ActorComponent.h"
#include "CUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDamaged, const float&, const float&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FHealed, const float&, const float&);
DECLARE_MULTICAST_DELEGATE(FHealthOver);

UCLASS(ClassGroup = (CatchUp), meta = (BlueprintSpawnableComponent))
class CATCHUP_API UCUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	FDamaged DamagedEvent;

	FHealed HealedEvent;
	
	FHealthOver HealthOverEvent;
	
private:

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0"))
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = "OnRep_CurrentHealth")
	float CurrentHealth;

	float LastHealthAmount;
	
protected:

	UCUHealthComponent();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:

	void Reset(const EGameRole& NewRole);
	
	void TakeDamage(const float& Amount);

	void Heal(const float& Amount);
	
private:

	void SetHealth(const float& NewHealth);

	UFUNCTION()
	void OnRep_CurrentHealth();
	
};
