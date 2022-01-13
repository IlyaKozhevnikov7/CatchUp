// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FDamaged);
DECLARE_MULTICAST_DELEGATE(FHealthOver);
DECLARE_MULTICAST_DELEGATE(FHealed);

struct FDamage;

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
	int32 MaxHealth;

	int32 CurrentHealth;

protected:

	UCUHealthComponent();

	virtual void BeginPlay() override;

public:

	void TakeDamage(const FDamage& Damage);

	void Heal(const int32& HealAmount);
	
private:

	void SetHealth(const int32& NewHealth);
	
};
