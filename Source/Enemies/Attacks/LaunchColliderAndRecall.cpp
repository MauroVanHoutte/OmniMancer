// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Attacks/LaunchColliderAndRecall.h"
#include <Engine/DamageEvents.h>

void ULaunchColliderAndRecall::TickAttack(float DeltaTime)
{
	Super::TickAttack(DeltaTime);

	if (bLaunching)
	{
		FVector NewLocation = FMath::VInterpConstantTo(LaunchedCollider->GetComponentLocation(), LaunchLocation, DeltaTime, LaunchSpeed);
		LaunchedCollider->SetWorldLocation(NewLocation);
		if (FVector::PointsAreNear(NewLocation, LaunchLocation, 0.01f))
		{
			LaunchCompleted();
		}
	}

	if (bRecalling)
	{
		FVector NewLocation = FMath::VInterpConstantTo(LaunchedCollider->GetComponentLocation(), Parent->GetComponentTransform().TransformPosition(PreLaunchTransform.GetLocation()), DeltaTime, RecallSpeed);
		LaunchedCollider->SetWorldLocation(NewLocation);
		FRotator NewRotation = FMath::RInterpConstantTo(LaunchedCollider->GetComponentRotation(), Parent->GetComponentTransform().TransformRotation(PreLaunchTransform.GetRotation()).Rotator(), DeltaTime, RecallSpeed);
		LaunchedCollider->SetWorldRotation(NewRotation);

		if (FVector::PointsAreNear(NewLocation, Parent->GetComponentTransform().TransformPosition(PreLaunchTransform.GetLocation()), 0.01f)
			&& NewRotation.Equals(Parent->GetComponentTransform().TransformRotation(PreLaunchTransform.GetRotation()).Rotator(), 0.01f))
		{
			RecallCompleted();
		}
	}
}

bool ULaunchColliderAndRecall::AreAttackRequirementsMet(AActor* Target)
{
	float DistanceSquared = FVector::DistSquared(Target->GetActorLocation(), OwningActor->GetActorLocation());
	return DistanceSquared < AttackRange * AttackRange;
}

void ULaunchColliderAndRecall::InitiateAttack(AActor* Target, const FVector& Location)
{
	Super::InitiateAttack(Target, Location);
	HitActors.Empty();

	LaunchedCollider = OwningActor->FindComponentByTag<UPrimitiveComponent>(LaunchedColliderTag);
	LaunchLocation = CalculateLaunchTarget();
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &ULaunchColliderAndRecall::StartLaunch);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, ChargeTime, false);
}

bool ULaunchColliderAndRecall::WasActorHitBefore(AActor* TriggeredActor, UPrimitiveComponent* ColliderComponent)
{
	return HitActors.Contains(TriggeredActor);
}

void ULaunchColliderAndRecall::OnHitTriggered(AActor* HitActor, UPrimitiveComponent* ColliderComponent)
{
	if (!WasActorHitBefore(HitActor, ColliderComponent) && ColliderComponent == LaunchedCollider)
	{
		FDamageEvent DamageEvent;
		HitActor->TakeDamage(Damage, DamageEvent, OwningActor->GetInstigatorController(), OwningActor);
		HitActors.Add(HitActor);
	}
}

void ULaunchColliderAndRecall::StartLaunch()
{
	if (!bUseLocationTarget)
		LaunchLocation = CalculateLaunchTarget();

	bLaunching = true;
	Parent = LaunchedCollider->GetAttachParent();
	PreLaunchTransform = LaunchedCollider->GetRelativeTransform();
	LaunchedCollider->SetCollisionProfileName(OverlapProfileName);
	LaunchedCollider->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	FRotator NewRotation = ((LaunchLocation - LaunchedCollider->GetComponentLocation()).GetSafeNormal()).ToOrientationRotator();
	LaunchedCollider->SetWorldRotation(NewRotation);
}

void ULaunchColliderAndRecall::LaunchCompleted()
{
	bLaunching = false;
	LaunchedCollider->SetCollisionProfileName(NoCollisionProfileName);
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &ULaunchColliderAndRecall::StartRecall);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, RecallDelay, false);
}

void ULaunchColliderAndRecall::StartRecall()
{
	bRecalling = true;
	if (bHitDuringRecall)
	{
		HitActors.Empty();
		LaunchedCollider->SetCollisionProfileName(OverlapProfileName);
	}
}

void ULaunchColliderAndRecall::RecallCompleted()
{
	bRecalling = false;
	LaunchedCollider->SetCollisionProfileName(NoCollisionProfileName);
	LaunchedCollider->AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform);
	OnAttackCompletedDelegate.Broadcast(this);
}

FVector ULaunchColliderAndRecall::CalculateLaunchTarget()
{
	FVector TargetCenterLocation = bUseLocationTarget ? TargetLocation : TargetActor->GetActorLocation();
	FVector OvershootDirection = (TargetCenterLocation - LaunchedCollider->GetComponentLocation()).GetSafeNormal();
	return TargetCenterLocation + OvershootDirection * OvershootDistance;
}
