// Fill out your copyright notice in the Description page of Project Settings.

#include "CUGameState.h"
#include "CUPlayerState.h"
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
	if (HasAuthority())
	{
		check(NewState != MatchState);
		
		MatchState = NewState;
		MatchStateChangedEvent.Broadcast(MatchState);

		UE_LOG(CULogGameState, Display, TEXT("[Server] %s"), *UEnum::GetValueAsString(NewState))
	}
}

void ACUGameState::OnMatchRestarted()
{
	WantingRestartMatch.Reset();

	for (auto PlayerState : PlayerArray)
	{
		Cast<ACUPlayerState>(PlayerState)->SetWantRestartMatch(false);
	}
}

void ACUGameState::OnStartMatchTicked_Implementation(const int32& Time)
{
	if (HasAuthority() == false)
		StartMatchTickedEvent.Broadcast(Time);
}

void ACUGameState::OnMatchTimeChanged_Implementation(const int32& NewTime)
{
	if (HasAuthority() == false)
		MatchTimeChangedEvent.Broadcast(NewTime);
}

void ACUGameState::AddWantingRestartMatch(const ACUPlayerState* Wanting)
{
	check(HasAuthority());

	WantingRestartMatch.Add(Wanting);

	// сейчас что бы перезапустить матч должны согласится все игроки

	if (WantingRestartMatch.Num() == PlayerArray.Num())
	{
		FTimerHandle TH;
		GetWorld()->GetTimerManager().SetTimer(TH,[this]()
		{
			ShouldRestartMatchEvent.Broadcast();	
		},
		1.f,
		false);

		UE_LOG(CULogGameState, Display, TEXT("ShouldRestartMatchEvent %i"), WantingRestartMatch.Num());
	}
}

void ACUGameState::OnRep_MatchState()
{
	MatchStateChangedEvent.Broadcast(MatchState);
}
