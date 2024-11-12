// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Moves/MovementMoves/LevitateMove.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Movement/ForceApplicationComponent.h"

void ULevitateMove::TickMove(float DeltaTime)
{
	if (!CheckMovementComp() || !CheckForceApplicationComp())
		return;

	if (bRising)
	{
		ForceApplicationComp->ApplyForce(FVector(0, 0, UpwardsForce));

		if (OwningActor->GetActorLocation().Z > StartZLevel + TargetLevitationHeight)
		{
			bRising = false;
			ForceApplicationComp->ClearAllForces();
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();

			if (TimeToStayAtPeakBeforeCompletion > 0)
			{
				FTimerDelegate Delegate;
				Delegate.BindUObject(this, &ULevitateMove::CallOnCompletion);
				TimerManager.SetTimer(CompletionTimerHandle, Delegate, TimeToStayAtPeakBeforeCompletion, false);
			}
			else
			{
				CallOnCompletion();
			}

			if (TimeToStayAtPeakBeforeCompletion + TimeToReenableAfterCompletion > 0)
			{
				FTimerDelegate Delegate;
				Delegate.BindUObject(this, &ULevitateMove::EnableGravity);
				TimerManager.SetTimer(GravityTimerHandle, Delegate, TimeToStayAtPeakBeforeCompletion + TimeToReenableAfterCompletion, false);
			}
			else
			{
				EnableGravity();
			}
		}
	}
}

void ULevitateMove::Execute(AActor* Target, const FVector& TargetLocation)
{
	if (!CheckMovementComp() || !CheckForceApplicationComp())
	{
		OnMoveCompletedDelegate.Broadcast(this);
		return;
	}

	StartZLevel = OwningActor->GetActorLocation().Z;
	bRising = true;
	MovementComp->GravityScale = 0.f;
}

void ULevitateMove::Interrupt()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(CompletionTimerHandle);
	TimerManager.ClearTimer(GravityTimerHandle);
	EnableGravity();
}

bool ULevitateMove::CheckMovementComp()
{
	if (IsValid(MovementComp))
		return true;
	else
		MovementComp = OwningActor->GetComponentByClass<UCharacterMovementComponent>();

	return IsValid(MovementComp);
}

bool ULevitateMove::CheckForceApplicationComp()
{
	if (IsValid(ForceApplicationComp))
		return true;
	else
		ForceApplicationComp = OwningActor->GetComponentByClass<UForceApplicationComponent>();

	return IsValid(ForceApplicationComp);
}

void ULevitateMove::CallOnCompletion()
{
	OnMoveCompletedDelegate.Broadcast(this);
}

void ULevitateMove::EnableGravity()
{
	if (CheckMovementComp())
		MovementComp->GravityScale = 1.f;
}
