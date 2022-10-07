// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpEffect.h"
#include "HealEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UHealEffect : public UPowerUpEffect
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter* target) override;
	virtual void Remove(AWizardCharacter* target) override;

	UPROPERTY(EditAnywhere)
	float HealAmount = 7.f;
};
