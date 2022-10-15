// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpEffect.h"
#include "FireRatePowerUpEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UFireRatePowerUpEffect : public UPowerUpEffect
{
	GENERATED_BODY()

	void Apply(AWizardCharacter* target) override;
	void Remove(AWizardCharacter* target) override;

	UPROPERTY(EditAnywhere)
	float Value = 0.5f;
};
