// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Moves/MovementMoves/MoveToTargetMove.h"
#include <GameFramework\CharacterMovementComponent.h>
#include "Movement/ForceApplicationComponent.h"

void UMoveToTargetMove::TickMove(float DeltaTime)
{
	FVector NewLocation = FMath::VInterpConstantTo(OwningActor->GetActorLocation(), Location, DeltaTime, MoveSpeed);
	OwningActor->SetActorLocation(NewLocation, false, nullptr, ETeleportType::ResetPhysics);
	if (FVector::PointsAreNear(NewLocation, Location, 0.1f))
	{
		OnMoveCompletedDelegate.Broadcast(this);
		FRotator Rotation = OwningActor->GetActorRotation();
		Rotation.Pitch = 0;
		Rotation.Roll = 0;
		OwningActor->SetActorRotation(Rotation);
		if (bDisableGravityForDuration && CheckMovementComp())
			MovementComp->GravityScale = 1.f;
		if (bDisableCollision)
			OwningActor->SetActorEnableCollision(true);
	}
}

void UMoveToTargetMove::Execute(AActor* Target, const FVector& TargetLocation)
{
	Location = TargetLocation;
	OwningActor->SetActorRotation((TargetLocation - OwningActor->GetActorLocation()).ToOrientationQuat());
	if (bDisableGravityForDuration && CheckMovementComp())
		MovementComp->GravityScale = 0.f;
	if (bDisableCollision)
		OwningActor->SetActorEnableCollision(false);
	if (!bUseSpeed)
		MoveSpeed = FVector::Distance(TargetLocation, OwningActor->GetActorLocation());
}

void UMoveToTargetMove::Interrupt()
{
	if (bDisableGravityForDuration && CheckMovementComp())
		MovementComp->GravityScale = 1.f;
	if (bDisableCollision)
		OwningActor->SetActorEnableCollision(true);
}

bool UMoveToTargetMove::CheckMovementComp()
{
	if (IsValid(MovementComp))
		return true;
	else
		MovementComp = OwningActor->GetComponentByClass<UCharacterMovementComponent>();

	return IsValid(MovementComp);
}
