// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUBaseWidget.generated.h"

UCLASS()
class CATCHUP_API UCUBaseWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	// добавить анимации активации и деактивации 

	bool bIsActive;
	
public:

	FORCEINLINE bool GetIsActive() const { return bIsActive; }
	
	void Activate();

	void Deactivate();
	
};
