#pragma once

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
	int32 StartMatchTicks = 3;
	
};

// Player

UENUM()
enum class EGameRole : uint8
{
	Runner = 0,
	Catcher,
	Indefined
};
