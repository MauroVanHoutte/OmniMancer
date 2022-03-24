// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "PowerUpEffect.generated.h"

class AWizardCharacter;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UPowerUpEffect : public UObject
{
	GENERATED_BODY()

public:

	void Apply(AWizardCharacter* target) const;
	void Remove(AWizardCharacter* target) const;

	UPROPERTY(EditDefaultsOnly)
	float Duration;

	UPROPERTY(EditDefaultsOnly)
	float Timer;
	
};
