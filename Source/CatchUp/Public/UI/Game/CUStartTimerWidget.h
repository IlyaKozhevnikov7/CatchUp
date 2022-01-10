// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Game/CUBaseWidget.h"
#include "CUStartTimerWidget.generated.h"

class UTextBlock;

UCLASS()
class CATCHUP_API UCUStartTimerWidget : public UCUBaseWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))	
	UWidgetAnimation* TickAnim;

	FWidgetAnimationDynamicEvent TickAnimFinishedCallBack;
	
public:

	void Deactivate() override;
	
	void UpdateTimer(const int32& Time);

private:

	virtual void NativeOnInitialized() override;
	
};
