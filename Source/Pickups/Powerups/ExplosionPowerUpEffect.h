// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpEffect.h"
#include "ExplosionPowerUpEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UExplosionPowerUpEffect : public UPowerUpEffect
{
	GENERATED_BODY()
	
	void Apply(AWizardCharacter* target) override;
	void Remove(AWizardCharacter* target) override;

	UPROPERTY(EditAnywhere)
	float Radius = 7;
	float Damage = 7;
};
