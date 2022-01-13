// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "EngineUtils.h"
#include "GameFramework/GameModeBase.h"
#include "CUGameMode.generated.h"

class APlayerStart;
class ACUCharacter;
class ACUPlayerState;

UCLASS()
class CATCHUP_API ACUGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TArray<ACUPlayerState*> PlayerStates;
	
	UPROPERTY(EditDefaultsOnly)
	FGameSettings GameSettings;

	TQueue<ACUCharacter*> CharactersPool;

	// Может быть нужно будет убрать в GameState
	EMatchState PreviousState;
	
	// temp here
	FTimerHandle StartMatchTimerHandle;

	int32 CurrentStartTick;
	
	int32 CurrentMatchTime;
	
public:

	const FGameSettings& GetGameSettings() const { return GameSettings; }
	
private:
	
	ACUGameMode();
	
	virtual void BeginPlay() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual bool AllowPausing(APlayerController* PC) override;
	
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate) override;
	
	virtual bool ClearPause() override;
	
	void RestartMatch();
	
	void InitCharactersPool();

	void RestartAllPlayers();
	
	virtual void RestartPlayer(AController* NewPlayer) override;

	void SetupPlayer(AController* Controller, APawn* Pawn, const FTransform& InitialTrnsform);

	void GiveOutRoles();

	void SelectCatchers();

	void SelectRunners();

	void ChangeMatchState(const EMatchState& NewState);

	void TickStartMatch();

	AActor* FindPlayerStartByRole(const EGameRole& GameRole) const;
};
