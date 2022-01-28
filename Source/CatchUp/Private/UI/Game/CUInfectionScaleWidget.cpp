// Fill out your copyright notice in the Description page of Project Settings.

#include "CUInfectionScaleWidget.h"
#include "Components/ProgressBar.h"

void UCUInfectionScaleWidget::OnHealthChanged(const float& NewHealth, const float& MaxHealth)
{
	InfectionScale->SetPercent(NewHealth / MaxHealth);
}
