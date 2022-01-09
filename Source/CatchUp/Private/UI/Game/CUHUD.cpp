// Fill out your copyright notice in the Description page of Project Settings.

#include "CUHUD.h"
#include "CUBaseWidget.h"
#include "CUGameState.h"
#include "CUPlayerController.h"
#include "CUPlayerState.h"
#include "CUTimerWidget.h"

DEFINE_LOG_CATEGORY_STATIC(CULogHUD, All, All);

void ACUHUD::BeginPlay()
{
	Super::BeginPlay();

	for (auto Class : RoleWidgetClasses)
		AddWidgetTo(Class.Value, Class.Key, RoleWidgets);

	for (auto Class : AdditionalWidgetClasses)
		AddWidgetTo(Class.Value, Class.Key, AdditionalWidgets);
	
	auto GameState = GetWorld()->GetGameState<ACUGameState>();
	check(GameState);

	GameState->MatchStateChangedEvent.AddUObject(this, &ACUHUD::OnMatchStateChanged);
	GameState->MatchTimeChangedEvent.AddUObject(this, &ACUHUD::OnMatchTimeChanged);
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
	ActivateRoleWidget(NewRole);
}

void ACUHUD::OnMatchStateChanged(const EMatchState& NewState)
{
	switch (NewState)
	{
	case EMatchState::InProgress:
		{
			ActivateAdditionalWidget(EAdditionWidget::Timer);
		}
	}
}

void ACUHUD::OnMatchTimeChanged(const int32& NewTime)
{
	Cast<UCUTimerWidget>(AdditionalWidgets[EAdditionWidget::Timer])->UpdateTimer(NewTime);
}

void ACUHUD::ActivateRoleWidget(const EGameRole& GameRole)
{
	if (ensureMsgf(RoleWidgets.Contains(GameRole), TEXT("Key [%s] is not exist in RoleWodgets"), *UEnum::GetValueAsString(GameRole)))
	{
 		if (ensureMsgf(CurrentRoleWidget == RoleWidgets[GameRole] || CurrentRoleWidget == nullptr, TEXT("CurrentRoleWidget is already %s widget"), *UEnum::GetValueAsString(GameRole)))
			return;
		
		if (CurrentRoleWidget != nullptr)
			CurrentRoleWidget->Deactivate();
	
		CurrentRoleWidget = RoleWidgets[GameRole];
		CurrentRoleWidget->Activate();
	}
}

void ACUHUD::ActivateAdditionalWidget(const EAdditionWidget& Type)
{
	if (ensureMsgf(AdditionalWidgets.Contains(Type), TEXT("Key [%s] is not exist in AdditionalWodgets"), *UEnum::GetValueAsString(Type)))
	{
		if (ensureMsgf(AdditionalWidgets[Type]->GetIsActive() == false, TEXT("Widget %s is already active"), *AdditionalWidgets[Type]->GetName()))
		{
			AdditionalWidgets[Type]->Activate();
		}
	}
}

void ACUHUD::DeactivateAdditionalWidget(const EAdditionWidget& Type)
{
	if (ensureMsgf(AdditionalWidgets.Contains(Type), TEXT("Key [%s] is not exist in AdditionalWodgets"), *UEnum::GetValueAsString(Type)))
	{
		if (ensureMsgf(AdditionalWidgets[Type]->GetIsActive(), TEXT("Widget %s is already deactivate"), *AdditionalWidgets[Type]->GetName()))
		{
			AdditionalWidgets[Type]->Deactivate();
		}
	}
}
