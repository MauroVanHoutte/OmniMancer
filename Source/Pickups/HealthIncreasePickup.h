// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BaseInteractPickup.h"
#include "HealthIncreasePickup.generated.h"

UCLASS()
class UNREALPROJECT_API AHealthIncreasePickup : public ABaseInteractPickup
{
	GENERATED_BODY()

private:
	void OnInteractAction_Implementation(AActor* Player) override;

	UPROPERTY(EditDefaultsOnly)
	float BonusHealth = 25;
};
