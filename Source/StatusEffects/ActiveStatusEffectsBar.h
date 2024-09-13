// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActiveStatusEffectsBar.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALPROJECT_API UActiveStatusEffectsBar : public UUserWidget
{
	GENERATED_BODY()
public:
	void AddStatusEffect(class UBaseStatusEffect* StatusEffect);
	void RemoveStatusEffect(class UBaseStatusEffect* StatusEffect);
	void UpdateTimers(const TArray<class UBaseStatusEffect*> StatusEffects);

private:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* StatusEffectsBox;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCircularProgressBar> WidgetClass;
	UPROPERTY(Transient)
	TMap<class UBaseStatusEffect*, class UCircularProgressBar*> ActiveStatusEffects;
};
