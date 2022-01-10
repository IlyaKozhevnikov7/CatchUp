// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBaseWidget.h"

void UCUBaseWidget::Activate()
{
	if (ActivationAnim != nullptr)
		PlayAnimation(ActivationAnim);
	else
		SetVisibility(ESlateVisibility::HitTestInvisible);
	
	bIsActive = true;
}

void UCUBaseWidget::Deactivate()
{
	if (DeactivationAnim != nullptr)
		PlayAnimation(DeactivationAnim);
	else
		SetVisibility(ESlateVisibility::Collapsed);

	bIsActive = false;
}
