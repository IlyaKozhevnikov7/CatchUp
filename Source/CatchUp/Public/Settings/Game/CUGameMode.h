// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/GameModeBase.h"
#include "CUGameMode.generated.h"

class ACUCharacter;

UCLASS()
class CATCHUP_API ACUGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TSet<APlayerController*> Players;
	
	UPROPERTY(EditDefaultsOnly)
	FGameSettings GameSettings;

	TQueue<ACUCharacter*> CharactersPool;

	FTimerHandle StartMatchTimerHandle;
	
public:

	const FGameSettings& GetGameSettings() const { return GameSettings; }
	
private:
	
	ACUGameMode();
	
	virtual void BeginPlay() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	void InitCharactersPool();

	void RestartAllPlayers();
	
	virtual void RestartPlayer(AController* NewPlayer) override;

	void SetupPlayer(AController* Controller, APawn* Pawn, const FTransform& InitialTrnsform);

	void GiveCharacterTo(AController* Playerm, const float& Delay);

	void ChangeMatchState(const EMatchState& NewState);

	void TickStartMatch();
};
