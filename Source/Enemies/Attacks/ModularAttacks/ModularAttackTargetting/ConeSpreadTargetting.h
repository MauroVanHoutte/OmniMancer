// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/ModularAttacks/ModularAttackTargetting/ModularAttackTargetting.h"
#include "ConeSpreadTargetting.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UConeSpreadTargetting : public UModularAttackTargettingBase
{
	GENERATED_BODY()

public:
	virtual void GatherTargets(const FVector& TargetLocation, TArray<AActor*>& outTargetActors, TArray<FVector>& outTargetLocations) override;

private:
	// Angle in Degrees
	UPROPERTY(EditAnywhere)
	float ConeHalfAngle = 45.f;
	// Number of target locations along the cones arc
	UPROPERTY(EditAnywhere)
	int SpreadCount = 9;
	// Non zero values will no longer be aiming at the target
	UPROPERTY(EditAnywhere)
	float AngleOffset = 0.f;
};
