// Fill out your copyright notice in the Description page of Project Settings.

#include "CUStartTimerWidget.h"
#include "Components/TextBlock.h"

void UCUStartTimerWidget::Deactivate()
{
	if (TickAnim->IsPlayableDirectly())
	{
		BindToAnimationFinished(TickAnim, TickAnimFinishedCallBack);
	}
	else
	{
		const auto RightCallBack = AnimationCallbacks.FindByPredicate([this](FAnimationEventBinding& Bind)
		{
			return Bind.Delegate.IsBoundToObject(this);
		});
		
		if (RightCallBack != nullptr)
			UnbindFromAnimationFinished(RightCallBack->Animation, TickAnimFinishedCallBack);
		
		Super::Deactivate();
	}
}

void UCUStartTimerWidget::UpdateTimer(const int32& Time)
{	
	TimerText->SetText(FText::AsNumber(Time));
	PlayAnimation(TickAnim);
}

void UCUStartTimerWidget::NativeOnInitialized()
{
	TickAnimFinishedCallBack.BindDynamic(this, &Super::Deactivate);
	
	Super::NativeOnInitialized();
}
