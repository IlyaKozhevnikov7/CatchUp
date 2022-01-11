// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/PlayerController.h"
#include "CUPlayerController.generated.h"

class ACUPlayerState;
class ACUHUD;

UCLASS()
class CATCHUP_API ACUPlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY()
	ACUPlayerState* CUPlayerState;

	UPROPERTY()
	ACUHUD* CUHUD;
	
public:

	UFUNCTION()
	void SetWantRestartMatch();
	
private:

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	virtual void OnRep_PlayerState() override;
	
	virtual void OnRep_Pawn() override;
	
	void HandleMatchState(const EMatchState& NewState);

	UFUNCTION(Server, Reliable, WithValidation)
	void SetWantRestartMatchServer();
	
#if WITH_EDITOR
	
	UFUNCTION(Exec)
	void ChangeGameRole(const EGameRole& NewRole);

	UFUNCTION(Server, Reliable)
	void ChrangeGameRoleServer(const EGameRole& NewRole);
	
#endif // WITH_EDITOR
	
};
