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

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))	
	UWidgetAnimation* ActivationAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))	
	UWidgetAnimation* DeactivationAnim;
	
	bool bIsActive;
	
public:

	FORCEINLINE bool IsActive() const { return bIsActive; }

	UFUNCTION()
	virtual void Activate();

	UFUNCTION()
	virtual void Deactivate();
	
};
