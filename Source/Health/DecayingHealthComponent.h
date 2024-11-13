// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Health/BaseHealthComponent.h"
#include "DecayingHealthComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UDecayingHealthComponent : public UBaseHealthComponent
{
	GENERATED_BODY()
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Heal(float HealAmount) override;

protected:
	UFUNCTION()
	void StartDecaying();

	UPROPERTY(EditDefaultsOnly)
	float DecayRate = 0.3f;
	UPROPERTY(EditDefaultsOnly)
	float DecayDelay = 3.f;
	bool bIsDecaying = true;
	FTimerHandle DecayDelayTimer;
};
