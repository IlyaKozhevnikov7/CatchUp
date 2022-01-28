// Fill out your copyright notice in the Description page of Project Settings.

#include "CUHUD.h"
#include "CUBaseWidget.h"
#include "CUCharacter.h"
#include "CUGameState.h"
#include "CUHealthComponent.h"
#include "CUInfectionScaleWidget.h"
#include "CUMatchEndWidget.h"
#include "CUPlayerController.h"
#include "CUPlayerState.h"
#include "CUStartTimerWidget.h"
#include "CUTimerWidget.h"
#include "Components/Button.h"

DEFINE_LOG_CATEGORY_STATIC(CULogHUD, All, All);

void ACUHUD::BeginPlay()
{
	Super::BeginPlay();
	
	auto GameState = GetWorld()->GetGameState<ACUGameState>();
	check(GameState);

	GameState->MatchStateChangedEvent.AddUObject(this, &ACUHUD::OnMatchStateChanged);
	GameState->StartMatchTickedEvent.AddUObject(this, &ACUHUD::OnStartMatchTicked);
	GameState->MatchTimeChangedEvent.AddUObject(this, &ACUHUD::OnMatchTimeChanged);

	UE_LOG(CULogHUD, Display, TEXT("BeginPlay"));
}

void ACUHUD::Init(ACUPlayerController* Controller)
{
	InitWidgets();
	
	// бинды к PlayerController

	Cast<UCUMatchEndWidget>(AdditionalWidgets[EAdditionWidget::End])->GetRestartButtonClickEvent().AddDynamic(Controller, &ACUPlayerController::SetWantRestartMatch);
	
	// бинды к PlayerState
	const auto PlayerState = Controller->GetPlayerState<ACUPlayerState>();
	check(PlayerState);

	PlayerState->GameRoleChangedEvent.AddUObject(this, &ACUHUD::OnGameRoleChanged);
}

void ACUHUD::InitWidgets()
{
	for (auto Class : RoleWidgetClasses)
		AddWidgetTo(Class.Value, Class.Key, RoleWidgets);

	for (auto Class : AdditionalWidgetClasses)
		AddWidgetTo(Class.Value, Class.Key, AdditionalWidgets);
}

void ACUHUD::OnNewCharacter(ACUCharacter* Character)
{
	if (auto Health = Character->FindComponentByClass<UCUHealthComponent>())
	{
		const auto InfectionScaleWidget = Cast<UCUInfectionScaleWidget>(AdditionalWidgets[EAdditionWidget::InfectionScale]);
		
		Health->DamagedEvent.AddUObject(InfectionScaleWidget, &UCUInfectionScaleWidget::OnHealthChanged);
		Health->HealedEvent.AddUObject(InfectionScaleWidget, &UCUInfectionScaleWidget::OnHealthChanged);
	}
}

void ACUHUD::OnGameRoleChanged(const EGameRole& NewRole)
{
	ActivateRoleWidget(NewRole);
}

void ACUHUD::OnMatchStateChanged(const EMatchState& NewState)
{
#define ACTIVATE_ADDITIONAL_WIDGETS_ONLY(...) ActivateAdditionalWidgetsOnly({__VA_ARGS__});
	
	switch (NewState)
	{
	case EMatchState::PreStart:
		break;
		
	case EMatchState::Start:
			ACTIVATE_ADDITIONAL_WIDGETS_ONLY(EAdditionWidget::StartTimer);
		break;
		
	case EMatchState::InProgress:
			ACTIVATE_ADDITIONAL_WIDGETS_ONLY(EAdditionWidget::GameTimer, EAdditionWidget::InfectionScale);
		break;

	case EMatchState::Paused:
			ActivateAdditionalWidget(EAdditionWidget::Pause);
		break;

	case EMatchState::Ended:
			ActivateAdditionalWidget(EAdditionWidget::End);
		break;

	default:
		checkNoEntry();
	}
}

void ACUHUD::OnMatchTimeChanged(const int32& NewTime)
{
	Cast<UCUTimerWidget>(AdditionalWidgets[EAdditionWidget::GameTimer])->UpdateTimer(NewTime);
}

void ACUHUD::OnStartMatchTicked(const int32& Tick)
{
	Cast<UCUStartTimerWidget>(AdditionalWidgets[EAdditionWidget::StartTimer])->UpdateTimer(Tick);
}

void ACUHUD::ActivateRoleWidget(const EGameRole& GameRole)
{
	if (ensureMsgf(RoleWidgets.Contains(GameRole), TEXT("Key [%s] is not exist in RoleWodgets"), *UEnum::GetValueAsString(GameRole)))
	{
 		if (CurrentRoleWidget == RoleWidgets[GameRole])
			return;
		
		if (CurrentRoleWidget != nullptr)
			CurrentRoleWidget->Deactivate();
	
		CurrentRoleWidget = RoleWidgets[GameRole];
		CurrentRoleWidget->Activate();
	}
}

void ACUHUD::ActivateAdditionalWidget(const EAdditionWidget& Type) const
{
	check(AdditionalWidgets.Contains(Type) && AdditionalWidgets[Type]->IsActive() == false);

	AdditionalWidgets[Type]->Activate();
}

void ACUHUD::DeactivateAllAdditionalWidgets() const
{
	for (auto Widget : AdditionalWidgets)
	{
		if (Widget.Value->IsActive())
			Widget.Value->Deactivate();
	}
}
