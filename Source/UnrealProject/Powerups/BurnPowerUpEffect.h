// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpEffect.h"
#include "BurnPowerUpEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UBurnPowerUpEffect : public UPowerUpEffect
{
	GENERATED_BODY()
	
	void Apply(AWizardCharacter* target) override;
	void Remove(AWizardCharacter* target) override;

	float Interval = 1;
	float Duration = 5;
	float Damage = 2;
};
