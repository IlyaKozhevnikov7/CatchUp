// Fill out your copyright notice in the Description page of Project Settings.

#include "CURunnerWidget.h"
#include "Components/ProgressBar.h"

void UCURunnerWidget::OnHealthChanged(const float& NewHealth, const float& MaxHealth)
{
	Health->SetPercent(NewHealth / MaxHealth);
}
