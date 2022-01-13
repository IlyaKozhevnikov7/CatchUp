﻿#pragma once

#include "CatchUpTypes.generated.h"

// GameMode

UENUM()
enum class EMatchState : uint8 // возможно разделить EGameState на EGameState и ERoundState 
{
	WaitingForPlayers = 0,
	PreStart, // задержка перед раундом
	Start,
	InProgress,
	Paused,
	Ended
};

USTRUCT()
struct FGameSettings
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1"))
	int32 PlayerNum = 2;
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0"))
	int32 CatcherNum = 1;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0"))
	int32 StartMatchTicks = 5;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "300"))
	int32 MatchTime = 120;
	
};

// Player

UENUM()
enum class EGameRole : uint8
{
	Runner = 0,
	Catcher,
	Indefined
};

// HUD

UENUM()
enum class EAdditionWidget : uint8
{
	GameTimer = 0,
	StartTimer,
	Pause,
	End
};

// Controller

UENUM()
enum class EInputMode : uint8
{
	Game = 0,
	UI
};

// Hit Types

class ACUCharacter;

USTRUCT()
struct FHitBaseModifier
{
	GENERATED_BODY()

	friend struct FHit;

private:

	virtual void Process(ACUCharacter* Target) {  }

};

USTRUCT()
struct FHit
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly)
	TSet<FHitBaseModifier> Modifiers;

public:
	
	void Process(ACUCharacter* Target) const
	{
		ensure(Modifiers.Num() > 0);
		
		for (auto Modifier : Modifiers)
			Modifier.Process(Target);
	}
};

USTRUCT()
struct FDamageModifier : public FHitBaseModifier
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0"))
	float Damage = 10.f;

private:

	virtual void Process(ACUCharacter* Target) override;
	
};

