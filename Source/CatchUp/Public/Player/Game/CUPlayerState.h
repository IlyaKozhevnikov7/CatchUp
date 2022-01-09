// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/PlayerState.h"
#include "CUPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FGameRoleChanged, const EGameRole&);

UCLASS()
class CATCHUP_API ACUPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	FGameRoleChanged GameRoleChangedEvent;
	
private:

	UPROPERTY(ReplicatedUsing = "OnRep_GameRole")
	EGameRole GameRole;
	
protected:
	
	ACUPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	void ChangeRole(const EGameRole& NewRole);
	
	bool IsCatchcer() const;

private:
	
	UFUNCTION()
	void OnRep_GameRole();
};
