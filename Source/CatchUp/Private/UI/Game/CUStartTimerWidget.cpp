// Fill out your copyright notice in the Description page of Project Settings.

#include "CUStartTimerWidget.h"
#include "Components/TextBlock.h"

void UCUStartTimerWidget::UpdateTimer(const int32& Time)
{	
	TimerText->SetText(FText::AsNumber(Time));
	PlayAnimation(TickAnim);
}
