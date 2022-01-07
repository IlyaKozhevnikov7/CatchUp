// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CUGameMode.generated.h"

class ACUCharacter;

USTRUCT()
struct FGameSettings
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1"))
	int32 PlayerNum = 2;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0"))
	int32 CatcherNum = 1;
	
};

UCLASS()
class CATCHUP_API ACUGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly)
	FGameSettings GameSettings;

	TQueue<ACUCharacter*> CharactersPool;

public:

	const FGameSettings& GetGameSettings() const { return GameSettings; }
	
private:
	
	ACUGameMode();

	virtual void BeginPlay() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	void InitCharactersPool();

	virtual void RestartPlayer(AController* NewPlayer) override;

	void SetupPlayer(AController* Controller, APawn* Pawn, const FTransform& InitialTrnsform);
};
