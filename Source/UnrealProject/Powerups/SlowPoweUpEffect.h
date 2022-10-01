// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpEffect.h"
#include "SlowPoweUpEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USlowPoweUpEffect : public UPowerUpEffect
{
	GENERATED_BODY()
	
	void Apply(AWizardCharacter* target) override;
	void Remove(AWizardCharacter* target) override;

	float Duration = 5;
	float Amount = 0.1f;
};
