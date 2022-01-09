// Fill out your copyright notice in the Description page of Project Settings.

#include "CUPlayerController.h"
#include "CUCharacter.h"
#include "CUGameState.h"
#include "CUHUD.h"
#include "CUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(CULogPlayerController, All, All);

ACUPlayerController::ACUPlayerController()
{
	
}

void ACUPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() == false)
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
}

void ACUPlayerController::Pause()
{
	Super::Pause();
}

void ACUPlayerController::ChangeGameRole(const EGameRole& NewRole)
{
	ChrangeGameRoleServer(NewRole);
}

void ACUPlayerController::ChrangeGameRoleServer_Implementation(const EGameRole& NewRole)
{
	GetPlayerState<ACUPlayerState>()->ChangeRole(NewRole);
}

