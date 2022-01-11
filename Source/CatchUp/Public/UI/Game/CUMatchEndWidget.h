// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBaseWidget.h"
#include "CUMatchEndWidget.generated.h"

class UButton;
class FOnButtonClickedEvent;

UCLASS()
class CATCHUP_API UCUMatchEndWidget : public UCUBaseWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;

public:

	FOnButtonClickedEvent& GetRestartButtonClickEvent() const;
	
};
