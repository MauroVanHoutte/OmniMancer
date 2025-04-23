// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Moves/MovementMoves/OrbitMove.h"
#include <AIController.h>

void UOrbitMove::TickMove(float DeltaTime)
{
	
	float Time = DeltaTime;
	if (Timer + DeltaTime > Duration)
	{
		Time = Duration - Timer;
		OnMoveCompletedDelegate.Broadcast(this);
	}

	Timer += Time;

	if (IsValid(TargetActor) && IsValid(OwningActor))
	{
		OwningActor->SetActorRotation((OwningActor->GetActorLocation() - TargetActor->GetActorLocation()).ToOrientationRotator());
		FHitResult HitResult{};
		OwningActor->AddActorLocalOffset(FVector(0, SidewaysSpeed, 0) * Time, true, &HitResult);
		GEngine->AddOnScreenDebugMessage(98278978, 2, FColor::Red, HitResult.ToString());
		if (HitResult.bBlockingHit)
		{
			SidewaysSpeed *= -1;
		}

		if (FVector::DistSquared(OwningActor->GetActorLocation(), TargetActor->GetActorLocation()) > DistanceToTarget * DistanceToTarget)
		{
			float DistanceOver = FVector::Distance(OwningActor->GetActorLocation(), TargetActor->GetActorLocation()) - DistanceToTarget;
			float MappedSpeed = FMath::GetMappedRangeValueClamped(TRange<float>(0.f, 300.f), TRange<float>(0.f, VerticalSpeed), DistanceOver);
			OwningActor->AddActorLocalOffset(FVector(-MappedSpeed, 0, 0) * Time);
		}
	}
}

void UOrbitMove::Execute(AActor* Target, const FVector& TargetLocation)
{
	Super::Execute(Target, TargetLocation);

	TargetActor = Target;
	Timer = 0.f;
	OwningActor->GetInstigatorController<AAIController>()->SetFocus(Target);
}
