// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeAttackObject.h"
#include <AIController.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "Engine/DamageEvents.h"

void UChargeAttackObject::OnEndPlay()
{
	OwningActor->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UChargeAttackObject::TickAttack(float DeltaTime)
{
	Super::TickAttack(DeltaTime);

	if (bCharging)
	{
		float DistanceToMove = ChargeSpeed * DeltaTime;
		if (DistanceToMove + ChargedDistance > Range)
		{
			DistanceToMove = Range - ChargedDistance;
			bCharging = false;
		}
		else
		{
			ChargedDistance += DistanceToMove;
		}

		OwningActor->AddActorLocalOffset(FVector(DistanceToMove, 0, 0), true);

		if (!bCharging)
		{
			for (UPrimitiveComponent* Comp : HurtBoxes)
			{
				Comp->SetCollisionProfileName(NoCollisionProfileName);
			}

			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &UChargeAttackObject::CooldownCompleted);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, CooldownTime, false);
		}
	}
}

bool UChargeAttackObject::AreAttackRequirementsMet(AActor* Target)
{
	float DistSquared = FVector::DistSquared(OwningActor->GetActorLocation(), Target->GetActorLocation());
	if (DistSquared < Range * Range)
	{
		FHitResult Hit;
		TArray<AActor*> IgnoreActors;
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
			OwningActor->GetActorLocation(),
			Target->GetActorLocation(),
			75,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel4),
			true,
			IgnoreActors,
			EDrawDebugTrace::None,
			Hit,
			true);
		if (!Hit.bBlockingHit)
		{
			return true;
		}
	}

	return false;
}

bool UChargeAttackObject::WasActorHitBefore(AActor* TriggeredActor)
{
	return HitActors.Contains(TriggeredActor);
}

void UChargeAttackObject::OnHitTriggered(AActor* HitActor)
{
	HitActors.Add(HitActor);
	FDamageEvent DamageEvent;
	HitActor->TakeDamage(Damage, DamageEvent, OwningActor->GetInstigatorController(), OwningActor);
}

void UChargeAttackObject::InitiateAttack(AActor* Target)
{
	Super::InitiateAttack(Target);

	if (HurtBoxes.IsEmpty())
	{
		TArray<UActorComponent*> TaggedComponents = OwningActor->GetComponentsByTag(UPrimitiveComponent::StaticClass(), ColliderTag);
		for (UActorComponent* TaggedComponent : TaggedComponents)
		{
			UPrimitiveComponent* Collider = Cast<UPrimitiveComponent>(TaggedComponent);
			if (IsValid(Collider))
			{
				HurtBoxes.Add(Collider);
			}
		}
	}

	HitActors.Empty();
	FVector TargetLocation = Target->GetActorLocation();
	TargetLocation.Z = 0;
	FVector OwnerLocation = OwningActor->GetActorLocation();
	OwnerLocation.Z = 0;
	OwningActor->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(OwnerLocation, TargetLocation));
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UChargeAttackObject::StartCharge);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, ChargeUpTime, false);
	Cast<APawn>(OwningActor)->GetController<AAIController>()->SetFocus(nullptr);
}

void UChargeAttackObject::InterruptAttack()
{
	Super::InterruptAttack();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	bCharging = false;
}

void UChargeAttackObject::StartCharge()
{
	bCharging = true;
	ChargedDistance = 0.f;

	for (UPrimitiveComponent* Comp : HurtBoxes)
	{
		Comp->SetCollisionProfileName(OverlapProfileName);
	}
}

void UChargeAttackObject::CooldownCompleted()
{
	Cast<APawn>(OwningActor)->GetController<AAIController>()->SetFocus(TargetActor);
	OnAttackComponentCompletedDelegate.Broadcast(this);
}
