// Fill out your copyright notice in the Description page of Project Settings.

#include "CUPlayerState.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(CULogPlayerState, All, All);

void ACUPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACUPlayerState, GameRole);
}

void ACUPlayerState::ChangeRole(const EGameRole& NewRole)
{	
	if (GameRole != NewRole)
	{
		GameRole = NewRole;
		GameRoleChangedEvent.Broadcast(GameRole); // только для сервера
	}
}

void ACUPlayerState::OnRep_GameRole()
{
	GameRoleChangedEvent.Broadcast(GameRole);
}
