// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Attacks/ModularAttacks/ModularAttackTargetting/ConeSpreadTargetting.h"

void UConeSpreadTargetting::GatherTargets(const FVector& TargetLocation, TArray<AActor*>& outTargetActors, TArray<FVector>& outTargetLocations)
{
	int HalfSpreadCount = FMath::TruncToInt(SpreadCount/2.f);
	float AngleIncrement = FMath::DegreesToRadians(ConeHalfAngle / HalfSpreadCount);
	FVector Direction = TargetLocation - OwningActor->GetActorLocation();
	float Distance = Direction.Length();
	Direction.Z = 0;
	Direction.Normalize();
	float TargetAngle = FMath::Atan2(Direction.X, Direction.Y) + FMath::DegreesToRadians(AngleOffset);
	float CurrentAngleIncrement = AngleIncrement;

	if (SpreadCount % 2 == 1)
	{
		outTargetLocations.Add(TargetLocation);
	}
	else
	{
		CurrentAngleIncrement /= 2;
	}

	for (int i = 1; i <= HalfSpreadCount; i++)
	{
		outTargetLocations.Add(OwningActor->GetActorLocation() + FVector(FMath::Sin(TargetAngle + CurrentAngleIncrement), FMath::Cos(TargetAngle + CurrentAngleIncrement), 0) * Distance);
		outTargetLocations.Add(OwningActor->GetActorLocation() + FVector(FMath::Sin(TargetAngle - CurrentAngleIncrement), FMath::Cos(TargetAngle - CurrentAngleIncrement), 0) * Distance);
		CurrentAngleIncrement += AngleIncrement;
	}
}
