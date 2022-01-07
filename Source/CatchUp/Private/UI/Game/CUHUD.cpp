// Fill out your copyright notice in the Description page of Project Settings.

#include "CUHUD.h"
#include "CUBaseUserWidget.h"
#include "CUPlayerController.h"
#include "CUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(CULogHUD, All, All);

void ACUHUD::BeginPlay()
{
	Super::BeginPlay();

	for (auto Class : RoleWidgetClasses)
	{
		auto NewWidget = CreateWidget<UCUBaseUserWidget>(GetWorld(), Class.Value);
		check(NewWidget);

		NewWidget->AddToViewport();
		NewWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		RoleWidgets.Add(Class.Key, NewWidget);
	}
}

void ACUHUD::Init(ACUPlayerController* Controller)
{
	// бинды к PlayerController

	// бинды к PlayerState
	const auto PlayerState = Controller->GetPlayerState<ACUPlayerState>();
	check(PlayerState);

	PlayerState->GameRoleChangedEvent.AddUObject(this, &ACUHUD::OnGameRoleChanged);
}

void ACUHUD::OnNewCharacter(ACUCharacter* Character)
{
	
}

void ACUHUD::OnGameRoleChanged(const EGameRole& NewRole)
{
	if (RoleWidgets.Contains(NewRole))
	{
		if (CurrentRoleWidget != nullptr)
			CurrentRoleWidget->Deactivate();
		
		CurrentRoleWidget = RoleWidgets[NewRole];
		CurrentRoleWidget->Activate();
	}
}
