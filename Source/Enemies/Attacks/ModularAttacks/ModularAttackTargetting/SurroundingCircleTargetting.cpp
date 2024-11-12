// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Attacks/ModularAttacks/ModularAttackTargetting/SurroundingCircleTargetting.h"

void USurroundingCircleTargetting::GatherTargets(const FVector& TargetLocation, TArray<AActor*>& outTargetActors, TArray<FVector>& outTargetLocations)
{
	FVector Direction = OwningActor->GetActorLocation() - TargetLocation;
	Direction.Z = 0;
	Direction.Normalize();
	float StartingAngle = FMath::Atan2(Direction.X, Direction.Y) + FMath::DegreesToRadians(AngleOffset);
	float AngleIncrement = FMath::DegreesToRadians(360.f / TargetCount);

	for (size_t i = 0; i < TargetCount; i++)
	{
		outTargetLocations.Add(TargetLocation + FVector(FMath::Sin(StartingAngle + AngleIncrement * i), FMath::Cos(StartingAngle + AngleIncrement * i), 0) * CircleRadius);
	}
}
