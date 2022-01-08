// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CatchUpTypes.h"
#include "CUGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FMatchStateChanged, const EMatchState&);
DECLARE_MULTICAST_DELEGATE_OneParam(FStartMatchTimeChanged, const float&);

UCLASS()
class CATCHUP_API ACUGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	FMatchStateChanged MatchStateChangedEvent;

	FStartMatchTimeChanged StartMatchTimeChangedEvent;
	
private:

	UPROPERTY(ReplicatedUsing = "OnRep_MatchState")
	EMatchState MatchState;

protected:
	
	ACUGameState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	FORCEINLINE const EMatchState& GetGameState() const { return MatchState; }
	
	void ChangeMatchState(const EMatchState& NewState);

	UFUNCTION(Client, Reliable)
	void OnStartMatchTicked(const int32& Time);

private:

	UFUNCTION()
	void OnRep_MatchState();

	void TickStartMatch(const int32& LastTime);
	
};
