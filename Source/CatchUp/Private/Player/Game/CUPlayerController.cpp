// Fill out your copyright notice in the Description page of Project Settings.

#include "CUPlayerController.h"
#include "CUCharacter.h"
#include "CUGameState.h"
#include "CUHUD.h"
#include "CUPlayerState.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(CULogPlayerController, All, All);

void ACUPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		CUPlayerState = GetPlayerState<ACUPlayerState>();
		check(CUPlayerState);
	}
	else
	{
		SetShowMouseCursor(false);

		// пока хз как отключить прием инпутов со стороны сервера
		GetWorld()->GetGameState<ACUGameState>()->MatchStateChangedEvent.AddUObject(this, &ACUPlayerController::HandleMatchState);
	}
}

void ACUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Pause", IE_Pressed, this, &APlayerController::Pause).bExecuteWhenPaused = true;;
	
	InputComponent->BindAxis("Turn", this, &APlayerController::AddYawInput);
	InputComponent->BindAxis("LookAt", this, &APlayerController::AddPitchInput);
}

void ACUPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	CUPlayerState = GetPlayerState<ACUPlayerState>();
	check(CUPlayerState);
	
	CUHUD = GetHUD<ACUHUD>();
	check(CUHUD);

	CUHUD->Init(this);
}

void ACUPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (GetPawn() != nullptr)
	{
		if (GetWorld()->GetGameState<ACUGameState>())
			HandleMatchState(GetWorld()->GetGameState<ACUGameState>()->GetGameState());
		
		CUHUD->OnNewCharacter(GetPawn<ACUCharacter>());	
	}
}

void ACUPlayerController::SetWantRestartMatch()
{
	if (CUPlayerState->IsWantRestartMatch() == false)
		SetWantRestartMatchServer();
}

void ACUPlayerController::HandleMatchState(const EMatchState& NewState)
{	
	if (NewState == EMatchState::Start)
	{
		if (GetPawn())
			GetPawn()->DisableInput(this);
	}
	else if (NewState == EMatchState::InProgress)
	{
		if (GetPawn())
			GetPawn()->EnableInput(this);
	}

	if (NewState == EMatchState::Paused)
	{
		if (GetPawn())
			GetPawn()->DisableInput(this);
	}

	if (NewState == EMatchState::Ended)
	{
		DisableInput(this);
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
		
		if (GetPawn())
			GetPawn()->DisableInput(this);
	}
}

void ACUPlayerController::SetWantRestartMatchServer_Implementation()
{
	GetWorld()->GetGameState<ACUGameState>()->AddWantingRestartMatch(GetPlayerState<ACUPlayerState>());

	CUPlayerState->SetWantRestartMatch(true);
}

bool ACUPlayerController::SetWantRestartMatchServer_Validate()
{
	return CUPlayerState->IsWantRestartMatch() == false;
}

void ACUPlayerController::ChangeGameRole(const EGameRole& NewRole)
{
	ChrangeGameRoleServer(NewRole);
}

void ACUPlayerController::ChrangeGameRoleServer_Implementation(const EGameRole& NewRole)
{
	GetPlayerState<ACUPlayerState>()->ChangeRole(NewRole);
}

