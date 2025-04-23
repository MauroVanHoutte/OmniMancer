// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/Attacks/RotateColliderAttackObject.h"
#include <Engine/DamageEvents.h>
#include "Kismet/KismetMathLibrary.h"

void URotateColliderAttackObject::TickAttack(float DeltaTime)
{
	Super::TickAttack(DeltaTime);

	if (bIsDashing)
	{
		float Time = DeltaTime;
		if (DashTimer + Time > DashDuration)
		{
			Time = DashDuration - DashTimer;
			DashCompleted();
		}
		DashTimer += Time;
		float DashSpeed = DashDistance / DashDuration;
		OwningActor->AddActorLocalOffset(FVector::ForwardVector * DashSpeed * Time);
	}

	if (bIsRotating)
	{
		float Time = DeltaTime;
		if (RotationDuration > 0)
		{
			if (RotationTimer + Time > RotationDuration)
			{
				Time = RotationDuration - RotationTimer;
				RotationCompleted();
			}
		}
		
		RotationTimer += Time;
		float RotationSpeed = RotationDuration > 0 ? DeltaRotationInDegrees / RotationDuration : DeltaRotationInDegrees;
		ColliderRoot->AddLocalRotation(FRotator(0, RotationSpeed * Time, 0));

		if (bRotateColliders)
		{
			for (UPrimitiveComponent* Collider : Colliders)
			{
				Collider->AddLocalRotation(FRotator(0, Time * ColliderRotationSpeed, 0));
			}
		}
	}
}

bool URotateColliderAttackObject::AreAttackRequirementsMet(AActor* Target)
{
	float DistanceSquared = FVector::DistSquared(Target->GetActorLocation(), OwningActor->GetActorLocation());
	return DistanceSquared < AttackRange * AttackRange;
}

void URotateColliderAttackObject::InitiateAttack(AActor* Target, const FVector& Location)
{
	Super::InitiateAttack(Target, Location);

	if (Colliders.IsEmpty() || !IsValid(ColliderRoot))
	{
		TArray<UActorComponent*> TaggedComponents = OwningActor->GetComponentsByTag(UPrimitiveComponent::StaticClass(), ColliderTag);
		for (UActorComponent* TaggedComponent : TaggedComponents)
		{
			UPrimitiveComponent* Collider = Cast<UPrimitiveComponent>(TaggedComponent);
			if (IsValid(Collider))
			{
				Colliders.Add(Collider);
			}
		}

		ColliderRoot = OwningActor->FindComponentByTag<USceneComponent>(ColliderRootTag);
	}

	if (IsValid(ColliderRoot))
	{
		RotationStartAngle = ColliderRoot->GetRelativeRotation().Yaw;
	}

	HitActors.Empty();
	bRotationCompleted = false;
	bDashCompleted = bDashDuringAttack ? false : true;

	if (bFaceTarget)
	{
		OwningActor->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(OwningActor->GetActorLocation(), bUseLocationTarget ? Location : Target->GetActorLocation()));
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (bDashDuringAttack)
	{
		if (DashStartDelay > 0)
		{
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &URotateColliderAttackObject::StartDash);
			TimerManager.SetTimer(DashStartTimer, Delegate, DashStartDelay, false);
		}
		else
		{
			StartDash();
		}
	}

	if (RotationStartDelay > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &URotateColliderAttackObject::StartRotation);
		TimerManager.SetTimer(RotationStartTimer, Delegate, RotationStartDelay, false);
	}
	else
	{
		StartRotation();
	}
}

bool URotateColliderAttackObject::WasActorHitBefore(AActor* TriggeredActor, UPrimitiveComponent* ColliderComponent)
{
	return !Colliders.Contains(ColliderComponent) || HitActors.Contains(TriggeredActor);
}

void URotateColliderAttackObject::OnHitTriggered(AActor* HitActor, UPrimitiveComponent* ColliderComponent)
{
	if (Colliders.Contains(ColliderComponent))
	{
		HitActors.Add(HitActor);
		OnAttackHitDelegate.Broadcast(this, OwningActor, HitActor);
		FDamageEvent DamageEvent;
		HitActor->TakeDamage(Damage, DamageEvent, OwningActor->GetInstigatorController(), OwningActor);

		if (MultipleHitsInterval > 0)
		{
			FTimerHandle Handle;
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &URotateColliderAttackObject::RemoveHitActor, HitActor);
			GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, MultipleHitsInterval, false);
		}
	}
}

void URotateColliderAttackObject::StartRotation()
{
	bIsRotating = true;
	RotationTimer = 0.f;

	for (UPrimitiveComponent* Collider : Colliders)
	{
		Collider->SetCollisionProfileName(OverlapProfileName);
	}
}

void URotateColliderAttackObject::StartDash()
{
	bIsDashing = true;
	DashTimer = 0.f;
}

void URotateColliderAttackObject::RotationCompleted()
{
	bIsRotating = false;
	bRotationCompleted = true;

	for (UPrimitiveComponent* Collider : Colliders)
	{
		Collider->SetCollisionProfileName(NoCollisionProfileName);
	}

	if (bDashCompleted)
	{
		if (AttackCompletionDelay > 0)
		{
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &URotateColliderAttackObject::AttackCompleted);
			TimerManager.SetTimer(AttackCompletionHandle, Delegate, AttackCompletionDelay, false);
		}
		else
		{
			AttackCompleted();
		}
	}
}

void URotateColliderAttackObject::DashCompleted()
{
	bIsDashing = false;
	bDashCompleted = true;

	if (bRotationCompleted)
	{
		if (AttackCompletionDelay > 0)
		{
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &URotateColliderAttackObject::AttackCompleted);
			TimerManager.SetTimer(AttackCompletionHandle, Delegate, AttackCompletionDelay, false);
		}
		else
		{
			AttackCompleted();
		}
	}
}

void URotateColliderAttackObject::AttackCompleted()
{
	OnAttackCompletedDelegate.Broadcast(this);
}

void URotateColliderAttackObject::RemoveHitActor(AActor* Actor)
{
	HitActors.Remove(Actor);
}
