// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpEffect.h"
#include "BouncePowerUpEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UBouncePowerUpEffect : public UPowerUpEffect
{
	GENERATED_BODY()
public:
	void Apply(AWizardCharacter* target) override;
	void Remove(AWizardCharacter* target) override;

	int Bounces = 3;
};
