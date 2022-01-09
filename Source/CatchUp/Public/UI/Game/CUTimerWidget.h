// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBaseWidget.h"
#include "CUTimerWidget.generated.h"

class UTextBlock;

UCLASS()
class CATCHUP_API UCUTimerWidget : public UCUBaseWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;
	
public:

	void UpdateTimer(const int32& Time);
	
};
