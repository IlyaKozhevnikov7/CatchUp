// Fill out your copyright notice in the Description page of Project Settings.

#include "CUPlayerController.h"
#include "CUCharacter.h"
#include "CUGameState.h"
#include "CUHUD.h"
#include "CUPlayerState.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(CULogPlayerController, All, All);

ACUPlayerController::ACUPlayerController()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
}

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
		InputMode = EInputMode::UI;
		SwitchInputMode(EInputMode::Game);

		// пока хз как отключить прием инпутов со стороны сервера
		GetWorld()->GetGameState<ACUGameState>()->MatchStateChangedEvent.AddUObject(this, &ACUPlayerController::HandleMatchState);
	}
}

void ACUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Pause", IE_Pressed, this, &APlayerController::Pause).bExecuteWhenPaused = true;
	
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
		SwitchInputMode(EInputMode::Game);
		
		if (GetPawn())
			GetPawn()->DisableInput(this);
	}
	else if (NewState == EMatchState::InProgress)
	{
		SwitchInputMode(EInputMode::Game);
		
		if (GetPawn())
			GetPawn()->EnableInput(this);
	}

	if (NewState == EMatchState::Paused)
	{
		SwitchInputMode(EInputMode::Game);
		
		if (GetPawn())
			GetPawn()->DisableInput(this);
	}

	if (NewState == EMatchState::Ended)
	{
		SwitchInputMode(EInputMode::UI);
		
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

void ACUPlayerController::SwitchInputMode(const EInputMode& Mode)
{
	if (InputMode == Mode)
		return;
	
	if (Mode == EInputMode::Game)
	{
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		if (GetPawn())
			GetPawn()->DisableInput(this); // иниче павн запомнит последний инпут и продолжит его обробатывать
		
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
	}

	InputMode = Mode;
}

void ACUPlayerController::ChangeGameRole(const EGameRole& NewRole)
{
	ChrangeGameRoleServer(NewRole);
}

void ACUPlayerController::ChrangeGameRoleServer_Implementation(const EGameRole& NewRole)
{
	GetPlayerState<ACUPlayerState>()->ChangeRole(NewRole);
}

