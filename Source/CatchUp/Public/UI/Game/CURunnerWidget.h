// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBaseWidget.h"
#include "CURunnerWidget.generated.h"

class UProgressBar;

UCLASS()
class CATCHUP_API UCURunnerWidget : public UCUBaseWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Health;

public:

	void Activate() override;
	
	void OnHealthChanged(const float& NewHealth, const float& MaxHealth);
	
};
