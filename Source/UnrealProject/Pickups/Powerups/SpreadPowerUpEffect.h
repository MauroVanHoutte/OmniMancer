// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpEffect.h"
#include "SpreadPowerUpEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USpreadPowerUpEffect : public UPowerUpEffect
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter* target) override;
	virtual void Remove(AWizardCharacter* target) override;

	UPROPERTY(EditAnywhere)
	int Value = 1;
};
