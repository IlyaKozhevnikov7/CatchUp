// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CatchUpTypes.h"
#include "CUGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FMatchStateChanged, const EMatchState&);
DECLARE_MULTICAST_DELEGATE_OneParam(FStartMatchTicked, const int32&);
DECLARE_MULTICAST_DELEGATE_OneParam(FMatchTimeChanged, const int32&);
DECLARE_MULTICAST_DELEGATE(FShouldRestartMatch);

class ACUPlayerState;

UCLASS()
class CATCHUP_API ACUGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	FMatchStateChanged MatchStateChangedEvent;

	FStartMatchTicked StartMatchTickedEvent;

	FMatchTimeChanged MatchTimeChangedEvent;

	FShouldRestartMatch ShouldRestartMatchEvent;
	
private:

	UPROPERTY(ReplicatedUsing = "OnRep_MatchState")
	EMatchState MatchState;

	UPROPERTY()
	TArray<const ACUPlayerState*> WantingRestartMatch;
	
protected:
	
	ACUGameState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	FORCEINLINE const EMatchState& GetGameState() const { return MatchState; }
	
	void ChangeMatchState(const EMatchState& NewState);

	UFUNCTION(NetMulticast, Reliable)
	void OnStartMatchTicked(const int32& Time);

	UFUNCTION(NetMulticast, Reliable)
	void OnMatchTimeChanged(const int32& NewTime);
	
	void AddWantingRestartMatch(const ACUPlayerState* Wanting);

private:

	UFUNCTION()
	void OnRep_MatchState();
	
};
