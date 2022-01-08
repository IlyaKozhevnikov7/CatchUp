 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "GameFramework/HUD.h"
#include "CUHUD.generated.h"

class UCUBaseUserWidget;
class ACUCharacter;
class ACUPlayerController;

UCLASS()
class CATCHUP_API ACUHUD : public AHUD
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly)
	TMap<EGameRole, TSubclassOf<UCUBaseUserWidget>> RoleWidgetClasses;

	TMap<EGameRole, UCUBaseUserWidget*> RoleWidgets;

	UPROPERTY()
	UCUBaseUserWidget* CurrentRoleWidget;
	
private:
	
	virtual void BeginPlay() override;

public:

	/* Срабатывает единожды когда создается контроллер (в данный момент у контроллера уже имеется плеер стейт) */ 
	void Init(ACUPlayerController* Controller);
	
	/* Срабатывает когда контроллер получает НОВОГО персонажа */ 
	void OnNewCharacter(ACUCharacter* Character);

private:

	void OnGameRoleChanged(const EGameRole& NewRole);	
};
