// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUBaseUserWidget.generated.h"

UCLASS()
class CATCHUP_API UCUBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void Activate();

	void Deactivate();
	
};
