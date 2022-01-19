// Fill out your copyright notice in the Description page of Project Settings.

#include "CUGameMode.h"
#include "CUCharacter.h"
#include "CUGameState.h"
#include "CUHUD.h"
#include "CUPlayerController.h"
#include "CUPlayerStart.h"
#include "CUPlayerState.h"
#include "CatchUp/CatchUpMacros.h"
#include "GameFramework/GameSession.h"

ACUGameMode::ACUGameMode()
{
	DefaultPawnClass = ACUCharacter::StaticClass();
	PlayerControllerClass = ACUPlayerController::StaticClass();
	PlayerStateClass = ACUPlayerState::StaticClass();
	HUDClass = ACUHUD::StaticClass();
	GameStateClass = ACUGameState::StaticClass();
}

void ACUGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitCharactersPool();

	GetGameState<ACUGameState>()->ShouldRestartMatchEvent.AddUObject(this, &ACUGameMode::RestartMatch);
}

void ACUGameMode::PostLogin(APlayerController* NewPlayer)
{
	GenericPlayerInitialization(NewPlayer);

	UWorld* World = GetWorld();
	
	NewPlayer->ClientCapBandwidth(NewPlayer->Player->CurrentNetSpeed);

	if (GameSession)
		GameSession->PostLogin(NewPlayer);

	PlayerStates.Add(NewPlayer->GetPlayerState<ACUPlayerState>());
	
	K2_PostLogin(NewPlayer);
	FGameModeEvents::GameModePostLoginEvent.Broadcast(this, NewPlayer);

	DELAY_WITH_PARAMS(this, &ACUGameMode::RestartPlayer, 1.f, Cast<AController>(NewPlayer));
	
	if (PlayerStates.Num() == GameSettings.PlayerNum)
		ChangeMatchState(EMatchState::PreStart);
}

bool ACUGameMode::AllowPausing(APlayerController* PC)
{
	return Super::AllowPausing(PC) && IsPaused() == false;
}

bool ACUGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const bool bResult = Super::SetPause(PC, CanUnpauseDelegate);

	if (bResult)
	{
		PreviousState = GetGameState<ACUGameState>()->GetGameState();
		ChangeMatchState(EMatchState::Paused);

		// без ForceNetUpdate MatchState не реплицируется 
		GameState->ForceNetUpdate();
	}
	
	return bResult;
}

bool ACUGameMode::ClearPause()
{
	const bool bResult = Super::ClearPause();

	if (bResult)
		ChangeMatchState(PreviousState);
	
	return bResult;
}

void ACUGameMode::RestartMatch()
{
	ChangeMatchState(EMatchState::Start);
	GetGameState<ACUGameState>()->OnMatchRestarted();
}

void ACUGameMode::InitCharactersPool()
{	
	for (int32 Index = 0; Index < GameSettings.PlayerNum; Index++)
	{
		const auto NewCharacter = GetWorld()->SpawnActor<ACUCharacter>(DefaultPawnClass);

		if (NewCharacter != nullptr)
			CharactersPool.Enqueue(NewCharacter);
	}
}

void ACUGameMode::RestartAllPlayers()
{
	for (auto PlayerState : PlayerStates)
		RestartPlayer(PlayerState->GetOwner<AController>());
}

void ACUGameMode::RestartPlayer(AController* NewPlayer)
{
	const auto StartSpot = FindPlayerStartByRole(NewPlayer->GetPlayerState<ACUPlayerState>()->GetGameRole());

	if (StartSpot == nullptr)
		return;
	
	if (NewPlayer->GetPawn() == nullptr)
	{
		ACUCharacter* FreePawn = nullptr;

		CharactersPool.Dequeue(FreePawn);

		if (FreePawn == nullptr)
			return;
		
		SetupPlayer(NewPlayer, FreePawn, StartSpot->GetTransform());

		FreePawn->ResetState();
	}
	else
	{
		// TODO выставить состояние персонажа по умолчанию
		
		NewPlayer->GetPawn()->SetActorLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
		
		NewPlayer->ClientSetRotation(StartSpot->GetActorRotation());
		NewPlayer->SetControlRotation(StartSpot->GetActorRotation());
	}
}

void ACUGameMode::SetupPlayer(AController* Controller, APawn* Pawn, const FTransform& InitialTrnsform)
{
	const FVector Location = InitialTrnsform.GetLocation();
	const FQuat Rotation = InitialTrnsform.GetRotation();
	
	Pawn->SetActorLocationAndRotation(Location, Rotation);
	Controller->SetPawn(Pawn);
	Controller->Possess(Pawn);

	Controller->ClientSetRotation(Rotation.Rotator());
	Controller->SetControlRotation(Rotation.Rotator());
}

void ACUGameMode::GiveOutRoles()
{
	for (auto PlayerState : PlayerStates)
		PlayerState->ChangeRole(EGameRole::Indefined);

	SelectCatchers();
	SelectRunners();
}

void ACUGameMode::SelectCatchers()
{
	if (GameSettings.CatcherNum == 0)
	{
		UE_LOG(LogGameMode, Warning, TEXT("CatcherNum equals zero"));
		return;
	}
	
	int32 SelectedCathersCount = 0;

	while (SelectedCathersCount < GameSettings.CatcherNum)
	{
		auto SelectedPlayer = PlayerStates[FMath::RandRange(0, PlayerStates.Num() - 1)];
		
		if (SelectedPlayer && SelectedPlayer->IsCatchcer() == false)
		{
			SelectedPlayer->ChangeRole(EGameRole::Catcher);
			SelectedCathersCount++;
		}
	}	
}

void ACUGameMode::SelectRunners()
{
	for (auto PlayerState : PlayerStates)
		if (PlayerState->IsCatchcer() == false)
			PlayerState->ChangeRole(EGameRole::Runner);
}

void ACUGameMode::ChangeMatchState(const EMatchState& NewState)
{
#define BREAK_IF_TIMER_ACTIVE(TimerHandle)							\
	if (GetWorldTimerManager().IsTimerActive(TimerHandle))			\
		break;														\

	switch (NewState)
	{
	case EMatchState::PreStart:
		{
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, [this] { ChangeMatchState(EMatchState::Start); }, GameSettings.PreStartMatchTicks, false);
		}
		break;

	case EMatchState::Start:
		{
			// при переходе в Start, таймер, если он был запущен, должен продожить с места отанова
			// тк в случае паузы таймеры тоже останавливаются
			// тоже самое и с таймерами в других состояниях
			BREAK_IF_TIMER_ACTIVE(StartMatchTimerHandle);
			
			GiveOutRoles();
			RestartAllPlayers();

			CurrentStartTick = GameSettings.StartMatchTicks;
			GetWorldTimerManager().SetTimer(StartMatchTimerHandle, this, &ACUGameMode::TickStartMatch, 1.f, true, 0.f);

			CurrentMatchTime = GameSettings.MatchTime - 1;
		}
		break;

	case EMatchState::InProgress:
		{
			static FTimerHandle MatchTimerHandle;

			BREAK_IF_TIMER_ACTIVE(MatchTimerHandle);
			
			GetWorld()->GetTimerManager().SetTimer(MatchTimerHandle, [this]()
			{
				GetGameState<ACUGameState>()->OnMatchTimeChanged(CurrentMatchTime);
				CurrentMatchTime--;
				
				if (CurrentMatchTime == -1)
				{
					GetWorld()->GetTimerManager().ClearTimer(MatchTimerHandle);
					ChangeMatchState(EMatchState::Ended);
				}
			},
			1.f,
			true,
			0.f);
		}
		break;
	}
	
	GetGameState<ACUGameState>()->ChangeMatchState(NewState);
}

void ACUGameMode::TickStartMatch()
{
	if (CurrentStartTick == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(StartMatchTimerHandle);
		ChangeMatchState(EMatchState::InProgress);
		return;
	}
	
	if (CurrentStartTick > 0)
	{
		GetGameState<ACUGameState>()->OnStartMatchTicked(CurrentStartTick);
		CurrentStartTick--;
	}
}

AActor* ACUGameMode::FindPlayerStartByRole(const EGameRole& GameRole) const
{
	TArray<ACUPlayerStart*> RightStarts;	
	
	for (auto Start : TActorRange<ACUPlayerStart>(GetWorld()))
	{
		if (GameRole == EGameRole::Indefined)
		{
			RightStarts.Add(Start);
			continue;
		}
		
		if (Start->GetRole() == GameRole)
			RightStarts.Add(Start);
	}

	return RightStarts[FMath::RandRange(0, RightStarts.Num() - 1)];
}
