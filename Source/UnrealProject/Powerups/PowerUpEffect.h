// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../WizardCharacter.h"

#include "PowerUpEffect.generated.h"
/**
 * 
 */
UCLASS(Abstract)
class UNREALPROJECT_API UPowerUpEffect : public UObject
{
	GENERATED_BODY()

public:

	virtual void Apply(AWizardCharacter* target) {};
	virtual void Remove(AWizardCharacter* target) {};

	UPROPERTY(EditDefaultsOnly)
	float Duration = 20;

	UPROPERTY(EditDefaultsOnly)
	float Timer = 0;
	
};
