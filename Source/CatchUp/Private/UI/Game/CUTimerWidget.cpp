// Fill out your copyright notice in the Description page of Project Settings.

#include "CUTimerWidget.h"
#include "Components/TextBlock.h"

void UCUTimerWidget::UpdateTimer(const int32& Time)
{
	const int32 Min = Time / 60;
	const int32 Sec = Time % 60;
	
	const FString TimeText = FString::Printf(TEXT("%02d : %02d"), Min, Sec);
	TimerText->SetText(FText::FromString(TimeText));
}
