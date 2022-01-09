// Fill out your copyright notice in the Description page of Project Settings.

#include "CUGameMode.h"
#include "CUCharacter.h"
#include "CUGameState.h"
#include "CUHUD.h"
#include "CUPlayerController.h"
#include "CUPlayerState.h"
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
}

void ACUGameMode::PostLogin(APlayerController* NewPlayer)
{
	GenericPlayerInitialization(NewPlayer);

	UWorld* World = GetWorld();
	
	NewPlayer->ClientCapBandwidth(NewPlayer->Player->CurrentNetSpeed);

	if (MustSpectate(NewPlayer))
	{
		NewPlayer->ClientGotoState(NAME_Spectating);
	}
	else
	{
		const FUniqueNetIdRepl& NewPlayerStateUniqueId = NewPlayer->PlayerState->GetUniqueId();
		
		if (NewPlayerStateUniqueId.IsValid())
			GetGameInstance()->AddUserToReplay(NewPlayerStateUniqueId.ToString());
	}

	if (GameSession)
		GameSession->PostLogin(NewPlayer);

	PlayerStates.Add(NewPlayer->GetPlayerState<ACUPlayerState>());
	
	K2_PostLogin(NewPlayer);
	FGameModeEvents::GameModePostLoginEvent.Broadcast(this, NewPlayer);

	GiveCharacterTo(NewPlayer, 2.f);

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
	const auto StartSpot = FindPlayerStart(NewPlayer);

	if (StartSpot == nullptr)
		return;
	
	if (NewPlayer->GetPawn() == nullptr)
	{
		ACUCharacter* FreePawn = nullptr;

		CharactersPool.Dequeue(FreePawn);

		if (FreePawn == nullptr)
			return;
		
		SetupPlayer(NewPlayer, FreePawn, StartSpot->GetTransform());

		FreePawn->OnActivated();
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

	K2_OnRestartPlayer(Controller);
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
		// log
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

void ACUGameMode::GiveCharacterTo(AController* Player, const float& Delay)
{
	const FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ACUGameMode::RestartPlayer, Cast<AController>(Player));
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, Delay, false);
}

void ACUGameMode::ChangeMatchState(const EMatchState& NewState)
{
	switch (NewState)
	{
	case EMatchState::PreStart:
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this] { ChangeMatchState(EMatchState::Start); }, 5.f, false);
		}
		break;

	case EMatchState::Start:
		{
			// при переходе в Start, таймер, если он был запущен, должен продожить с места отанова
			// тк в случае паузы таймеры тоже останавливаются
			// тоже самое и с таймерами в других состояниях
			if (GetWorld()->GetTimerManager().IsTimerActive(StartMatchTimerHandle))
				break;
			
			GiveOutRoles();
			RestartAllPlayers();

			GetWorld()->GetTimerManager().SetTimer(StartMatchTimerHandle, this, &ACUGameMode::TickStartMatch, 1.f, true);

			CurrentMatchTime = GameSettings.MatchTime - 1;
		}
		break;

	case EMatchState::InProgress:
		{
			static FTimerHandle MatchTimerHandle;

			if (GetWorld()->GetTimerManager().IsTimerActive(MatchTimerHandle))
				break;
			
			GetWorld()->GetTimerManager().SetTimer(MatchTimerHandle, [this]()
			{
				GetGameState<ACUGameState>()->OnMatchTimeChanged(CurrentMatchTime);
				CurrentMatchTime--;
				
				if (CurrentMatchTime == 0)
					GetWorld()->GetTimerManager().ClearTimer(MatchTimerHandle);
			},
			1.f,
			true);
		}
		break;
	}
	
	GetGameState<ACUGameState>()->ChangeMatchState(NewState);
}

void ACUGameMode::TickStartMatch()
{
	static int32 CurrentStartTick;
	
	if (CurrentStartTick == 0)
		CurrentStartTick = GameSettings.StartMatchTicks;
	
	if (CurrentStartTick > 0)
	{
		GetGameState<ACUGameState>()->OnStartMatchTicked(CurrentStartTick);
		CurrentStartTick--;

		if (CurrentStartTick == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(StartMatchTimerHandle);
			ChangeMatchState(EMatchState::InProgress);
		}
	}
}


