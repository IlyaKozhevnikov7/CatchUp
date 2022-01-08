// Fill out your copyright notice in the Description page of Project Settings.

#include "CUGameState.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(CULogGameState, All, All);

ACUGameState::ACUGameState()
{
	MatchState = EMatchState::WaitingForPlayers;
}

void ACUGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACUGameState, MatchState);
}

void ACUGameState::ChangeMatchState(const EMatchState& NewState)
{
	if (HasAuthority() && NewState != MatchState)
	{
		MatchState = NewState;
		MatchStateChangedEvent.Broadcast(MatchState);

		UE_LOG(CULogGameState, Display, TEXT("[Server] ChangeGameState %s"), *UEnum::GetValueAsString(NewState));
	}
}

void ACUGameState::OnStartMatchTicked_Implementation(const int32& Time)
{
	StartMatchTimeChangedEvent.Broadcast(Time);
}

void ACUGameState::OnRep_MatchState()
{
	MatchStateChangedEvent.Broadcast(MatchState);
}

void ACUGameState::TickStartMatch(const int32& LastTime)
{
	StartMatchTimeChangedEvent.Broadcast(LastTime);
}
