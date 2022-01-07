// Fill out your copyright notice in the Description page of Project Settings.

#include "CUPlayerController.h"
#include "CUHUD.h"
#include "CUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(CULogPlayerController, All, All);

ACUPlayerController::ACUPlayerController()
{
	
}

void ACUPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const FString RoleString = HasAuthority() ? "Server" : "Client";
	UE_LOG(CULogPlayerController, Display, TEXT("[%s] OnPossess"), *RoleString);
}

void ACUPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	
	const FString RoleString = HasAuthority() ? "Server" : "Client";
	UE_LOG(CULogPlayerController, Display, TEXT("[%s] OnUnPossess"), *RoleString);
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
}

void ACUPlayerController::ChrangeGameRole(const EGameRole& NewRole)
{
	ChrangeGameRoleServer(NewRole);
}

void ACUPlayerController::ChrangeGameRoleServer_Implementation(const EGameRole& NewRole)
{
	GetPlayerState<ACUPlayerState>()->ChangeRole(NewRole);
}

