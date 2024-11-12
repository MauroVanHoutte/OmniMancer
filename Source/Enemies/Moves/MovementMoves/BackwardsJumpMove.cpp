// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Moves/MovementMoves/BackwardsJumpMove.h"
#include "Movement/ForceApplicationComponent.h"

void UBackwardsJumpMove::TickMove(float DeltaTime)
{
	if (bActive)
	{
		float Speed = HorizontalDistance / Duration;
		OwningActor->AddActorLocalOffset(FVector(-1 * Speed * DeltaTime, 0, 0), true);
		Timer += DeltaTime;
		if (Timer > Duration)
		{
			bActive = false;
			OnMoveCompletedDelegate.Broadcast(this);
		}
	}
}

void UBackwardsJumpMove::OnBeginPlay(AActor* Owner)
{
	Super::OnBeginPlay(Owner);
}

void UBackwardsJumpMove::OnEndPlay()
{
	Super::OnEndPlay();
}

void UBackwardsJumpMove::Execute(AActor* Target, const FVector& TargetLocation)
{
	UForceApplicationComponent* ForceComponent = OwningActor->GetComponentByClass<UForceApplicationComponent>();
	if (IsValid(ForceComponent))
		ForceComponent->ApplyImpulse(FVector(0, 0, UpwardsForce));
	bActive = true;
	Timer = 0.f;
}

void UBackwardsJumpMove::Interrupt()
{
	bActive = false;
}
