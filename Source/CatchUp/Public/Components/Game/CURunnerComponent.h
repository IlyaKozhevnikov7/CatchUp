// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CURunnerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDamaged, const float&, const float&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FHealed, const float&, const float&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FCaught, AController* /* Runner */,  AController* /* Catcher */);

UCLASS(ClassGroup = (CatchUp), meta = (BlueprintSpawnableComponent))
class CATCHUP_API UCURunnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	FDamaged DamagedEvent;

	FHealed HealedEvent;
	
	FCaught CaughtEvent;
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "Catch Up|Health", meta = (ClampMin = "0"))
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = "OnRep_CurrentHealth")
	float CurrentHealth;

	float LastHealthAmount;
	
protected:

	UCURunnerComponent();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:

	void Reset();
	
	void TakeDamage(const float& Damage, AController* Instigator);

	void Heal(const float& Amount);
	
private:

	void SetHealth(const float& NewHealth);

	UFUNCTION()
	void OnRep_CurrentHealth();
	
};
