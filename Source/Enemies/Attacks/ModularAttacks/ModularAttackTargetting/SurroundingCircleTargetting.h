// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/ModularAttacks/ModularAttackTargetting/ModularAttackTargetting.h"
#include "SurroundingCircleTargetting.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USurroundingCircleTargetting : public UModularAttackTargettingBase
{
	GENERATED_BODY()

public:
	virtual void GatherTargets(const FVector& TargetLocation, TArray<AActor*>& outTargetActors, TArray<FVector>& outTargetLocations) override;

private:
	UPROPERTY(EditAnywhere)
	float CircleRadius = 600.f;
	UPROPERTY(EditAnywhere)
	int TargetCount = 6;
	// Offset in degrees, at 0 the first target is located on the line between target and owner
	UPROPERTY(EditAnywhere)
	float AngleOffset = 0.f;
};
