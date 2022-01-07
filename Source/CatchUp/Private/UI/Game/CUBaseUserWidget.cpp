// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBaseUserWidget.h"

void UCUBaseUserWidget::Activate()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UCUBaseUserWidget::Deactivate()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
