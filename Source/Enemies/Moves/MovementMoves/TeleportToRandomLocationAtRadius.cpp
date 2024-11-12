// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Moves/MovementMoves/TeleportToRandomLocationAtRadius.h"
#include <Kismet/KismetSystemLibrary.h>
#include "NavigationSystem.h"

void UTeleportToRandomLocationAtRadius::Execute(AActor* Target, const FVector& TargetLocation)
{
	TArray<FVector> Locations;
	GetValidTeleportLocations(Locations, Target);
	if (Locations.Num() > 0)
	{
		int Idx = FMath::RandRange(0, Locations.Num() - 1);
		OwningActor->SetActorLocation(Locations[Idx]);
	}

	OnMoveCompletedDelegate.Broadcast(this);
}

bool UTeleportToRandomLocationAtRadius::CanBeExecuted(AActor* Target)
{
	if (Super::CanBeExecuted(Target))
	{
		TArray<FVector> Locations;
		GetValidTeleportLocations(Locations, Target);
		if (Locations.Num() > 0)
			return true;
	}

	return false;
}

void UTeleportToRandomLocationAtRadius::GetValidTeleportLocations(TArray<FVector>& outLocations, const AActor* Target)
{
	outLocations.Empty(Samples);
	float AngleIncrement = 360.f / Samples;
	for (int i = 0; i < Samples; i++)
	{
		UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
		float IterationAngle = FMath::DegreesToRadians(AngleIncrement * i);
		FVector IterationDirection{FMath::Sin(IterationAngle), FMath::Cos(IterationAngle), 0};
		FVector EndLocation = Target->GetActorLocation() + IterationDirection * Radius;
		FNavLocation OutLocation;
		if (NavSystem->ProjectPointToNavigation(EndLocation, OutLocation, FVector(1000, 1000, 1000)))
		{
			FHitResult Hit;
			TArray<AActor*> IgnoreActors;
			UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
				Target->GetActorLocation(),
				EndLocation,
				75,
				UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel4),
				true,
				IgnoreActors,
				EDrawDebugTrace::ForDuration,
				Hit,
				true);

			if (!Hit.bBlockingHit)
			{
				outLocations.Add(OutLocation.Location);
			}
		}
	}
}
