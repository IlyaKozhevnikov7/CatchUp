// Fill out your copyright notice in the Description page of Project Settings.

#include "CUMatchEndWidget.h"
#include "Components/Button.h"

FOnButtonClickedEvent& UCUMatchEndWidget::GetRestartButtonClickEvent() const
{
	return RestartButton->OnClicked;
}
