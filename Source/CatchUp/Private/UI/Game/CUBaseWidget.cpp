// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBaseWidget.h"

void UCUBaseWidget::Activate()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	
	bIsActive = true;
}

void UCUBaseWidget::Deactivate()
{
	SetVisibility(ESlateVisibility::Collapsed);

	bIsActive = false;
}
