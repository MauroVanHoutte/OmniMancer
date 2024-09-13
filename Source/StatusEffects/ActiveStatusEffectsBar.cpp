// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffects/ActiveStatusEffectsBar.h"
#include "Components/WrapBox.h"
#include "StatusEffects/StatusEffect.h"
#include "UI/CircularProgressBar.h"

void UActiveStatusEffectsBar::AddStatusEffect(UBaseStatusEffect* StatusEffect)
{
	UCircularProgressBar* NewWidget = CreateWidget<UCircularProgressBar>(this, WidgetClass);
	NewWidget->SetImage(StatusEffect->Icon);
	StatusEffectsBox->AddChildToWrapBox(NewWidget);
	ActiveStatusEffects.Add(StatusEffect, NewWidget);
}

void UActiveStatusEffectsBar::RemoveStatusEffect(UBaseStatusEffect* StatusEffect)
{
	if (!ActiveStatusEffects.IsEmpty())
	{
		UCircularProgressBar* Widget = nullptr;
		ActiveStatusEffects.RemoveAndCopyValue(StatusEffect, Widget);
		if (IsValid(Widget))
		{
			StatusEffectsBox->RemoveChild(Widget);
			Widget->RemoveFromParent();
		}
	}
}

void UActiveStatusEffectsBar::UpdateTimers(const TArray<class UBaseStatusEffect*> StatusEffects)
{
	for (UBaseStatusEffect* StatusEffect : StatusEffects)
	{
		UCircularProgressBar** Widget = ActiveStatusEffects.Find(StatusEffect);
		if (Widget)
		{
			(*Widget)->SetFillPercentage(1 - (StatusEffect->RemainingDuration / StatusEffect->TotalDuration));
		}
	}
}
