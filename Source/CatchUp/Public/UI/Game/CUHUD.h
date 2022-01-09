 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "CUBaseWidget.h"
#include "GameFramework/HUD.h"
#include "CUHUD.generated.h"

class UCUBaseWidget;
class ACUCharacter;
class ACUPlayerController;

UCLASS()
class CATCHUP_API ACUHUD : public AHUD
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly)
	TMap<EGameRole, TSubclassOf<UCUBaseWidget>> RoleWidgetClasses;

	UPROPERTY(EditDefaultsOnly)
	TMap<EAdditionWidget, TSubclassOf<UCUBaseWidget>> AdditionalWidgetClasses;

	UPROPERTY()
	TMap<EGameRole, UCUBaseWidget*> RoleWidgets;
	
	UPROPERTY()
	TMap<EAdditionWidget, UCUBaseWidget*> AdditionalWidgets;

	UPROPERTY()
	UCUBaseWidget* CurrentRoleWidget;

	UPROPERTY()
	TSet<UCUBaseWidget*> ActiveAdditionalWidget;
	
private:
	
	virtual void BeginPlay() override;

public:

	/*
	 * Срабатывает единожды когда создается контроллер (в данный момент у контроллера уже имеется плеер стейт)
	 */ 
	void Init(ACUPlayerController* Controller);
	
	/*
	 * Срабатывает когда контроллер получает НОВОГО персонажа
	 * + по идие тут надо передаваать еще старый павн что бы худ отбиндился от него
	 */ 
	void OnNewCharacter(ACUCharacter* Character);

private:
	
	void OnGameRoleChanged(const EGameRole& NewRole);

	void OnMatchStateChanged(const EMatchState& NewState);
	
	void OnMatchTimeChanged(const int32& NewTime);

	void ActivateRoleWidget(const EGameRole& GameRole);

	void ActivateAdditionalWidget(const EAdditionWidget& Type);

	void DeactivateAdditionalWidget(const EAdditionWidget& Type);
	
	template<typename K>
	void AddWidgetTo(const TSubclassOf<UCUBaseWidget>& WidgetClass, const K& Key, TMap<K, UCUBaseWidget*>& To)
	{
		auto NewWidget = CreateWidget<UCUBaseWidget>(GetWorld(), WidgetClass);
		check(NewWidget);

		NewWidget->AddToViewport();
		NewWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		To.Add(Key, NewWidget);
	}
};



