// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Moves/Moves.h"
#include "TeleportToRandomLocationAtRadius.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UTeleportToRandomLocationAtRadius : public UBaseMove
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* Target, const FVector& TargetLocation) override;
	virtual bool CanBeExecuted(AActor* Target) override;

private:
	void GetValidTeleportLocations(TArray<FVector>& outLocations, const AActor* Target);

	UPROPERTY(EditAnywhere)
	float Radius = 800.f;
	// Number of directions to check for available spaces
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	int Samples = 12;
};
