// Fill out your copyright notice in the Description page of Project Settings.

#include "CUGameMode.h"
#include "CUCharacter.h"
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
}

void ACUGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitCharactersPool();
}

void ACUGameMode::PostLogin(APlayerController* NewPlayer)
{
	// Runs shared initialization that can happen during seamless travel as well
	GenericPlayerInitialization(NewPlayer);

	// Perform initialization that only happens on initially joining a server
	UWorld* World = GetWorld();

	NewPlayer->ClientCapBandwidth(NewPlayer->Player->CurrentNetSpeed);

	if (MustSpectate(NewPlayer))
	{
		NewPlayer->ClientGotoState(NAME_Spectating);
	}
	else
	{
		// If NewPlayer is not only a spectator and has a valid ID, add him as a user to the replay.
		const FUniqueNetIdRepl& NewPlayerStateUniqueId = NewPlayer->PlayerState->GetUniqueId();
		if (NewPlayerStateUniqueId.IsValid())
		{
			GetGameInstance()->AddUserToReplay(NewPlayerStateUniqueId.ToString());
		}
	}

	if (GameSession)
	{
		GameSession->PostLogin(NewPlayer);
	}

	// Notify Blueprints that a new player has logged in.  Calling it here, because this is the first time that the PlayerController can take RPCs
	K2_PostLogin(NewPlayer);
	FGameModeEvents::GameModePostLoginEvent.Broadcast(this, NewPlayer);

	const FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ACUGameMode::RestartPlayer, Cast<AController>(NewPlayer));
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, 1, false); // игрок получит персонажа через 1 сек
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
		
		FreePawn->OnActivated();
		
		SetupPlayer(NewPlayer, FreePawn, StartSpot->GetTransform());
	}
	else
	{
		// выставить состояние персонажа по умолчанию
		
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


