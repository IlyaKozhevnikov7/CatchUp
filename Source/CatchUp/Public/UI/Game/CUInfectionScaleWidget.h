// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBaseWidget.h"
#include "CUInfectionScaleWidget.generated.h"

class UProgressBar;

UCLASS()
class CATCHUP_API UCUInfectionScaleWidget : public UCUBaseWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* InfectionScale;

public:
	
	void OnDamaged(const float& NewHealth, const float& MaxHealth);
	
};
