// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/PlayerStart.h"
#include "CUPlayerStart.generated.h"

UCLASS()
class CATCHUP_API ACUPlayerStart : public APlayerStart
{
	GENERATED_BODY()

private:

	UPROPERTY(EditInstanceOnly)
	EGameRole ForRole;
	
};
