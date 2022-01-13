// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FDamaged);
DECLARE_MULTICAST_DELEGATE(FHealthOver);
DECLARE_MULTICAST_DELEGATE(FHealed);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	float CurrentHealth;

protected:

	UCUHealthComponent();

	virtual void BeginPlay() override;

public:

	void TakeDamage(const float& Amount);

	void Heal(const float& Amount);
	
private:

	void SetHealth(const float& NewHealth);
	
};
