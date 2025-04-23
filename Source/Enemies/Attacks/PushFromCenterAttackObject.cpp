// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/Attacks/PushFromCenterAttackObject.h"
#include "Movement/ForceApplicationComponent.h"
#include <Engine/DamageEvents.h> 

void UPushFromCenterAttackObject::TickAttack(float DeltaTime)
{
	if (bPushing)
	{
		float Speed = PushDistance / PushOutDuration;
		float Time = DeltaTime;
		if (PushTimer + DeltaTime > PushOutDuration)
		{
			Time = PushOutDuration - PushTimer;
			bPushing = false;
		}

		PushTimer += Time;

		for (UPrimitiveComponent* Collider : Colliders)
		{
			if (IsValid(Collider))
			{
				FVector Direction = IsValid(CenterComponent) ? (Collider->GetComponentLocation() - CenterComponent->GetComponentLocation()).GetSafeNormal() : Collider->GetRelativeLocation().GetSafeNormal();
				Collider->MoveComponent(Direction * Speed * Time, Collider->GetComponentRotation(), true);
			}
		}

		if (!bPushing)
		{
			for (UPrimitiveComponent* Collider : Colliders)
			{
				if (IsValid(Collider))
				{
					Collider->SetCollisionProfileName(NoCollisionProfileName);
				}
			}

			if (CooldownTime > 0)
			{
				FTimerDelegate Delegate;
				Delegate.BindUObject(this, &UPushFromCenterAttackObject::CompleteAttack);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, CooldownTime, false);
			}
			else
			{
				CompleteAttack();
			}
		}
	}
}

bool UPushFromCenterAttackObject::AreAttackRequirementsMet(AActor* Target)
{
	return AttackRange * AttackRange > FVector::DistSquared(Target->GetActorLocation(), OwningActor->GetActorLocation());
}

void UPushFromCenterAttackObject::InitiateAttack(AActor* Target, const FVector& Location)
{
	Super::InitiateAttack(Target, Location);

	HitActors.Empty();

	if (!IsValid(CenterComponent) || Colliders.IsEmpty())
	{
		CenterComponent = OwningActor->FindComponentByTag<USceneComponent>(CenterComponentTag);
		TArray<UActorComponent*> Components = OwningActor->GetComponentsByTag(UPrimitiveComponent::StaticClass(), ColliderTag);
		for (UActorComponent* Comp : Components)
		{
			if (UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Comp))
			{
				Colliders.Add(PrimitiveComp);
			}
		}
	}

	if (ChargeTime > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UPushFromCenterAttackObject::StartPush);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, ChargeTime, false);
	}
	else
	{
		StartPush();
	}
}

bool UPushFromCenterAttackObject::WasActorHitBefore(AActor* TriggeredActor, UPrimitiveComponent* ColliderComponent)
{
	return HitActors.Contains(TriggeredActor);
}

void UPushFromCenterAttackObject::OnHitTriggered(AActor* HitActor, UPrimitiveComponent* ColliderComponent)
{
	if (Colliders.Contains(ColliderComponent))
	{
		HitActors.Add(HitActor);
		FDamageEvent DamageEvent{};
		HitActor->TakeDamage(Damage, DamageEvent, OwningActor->GetInstigatorController(), OwningActor);
		UForceApplicationComponent* ForceComp = HitActor->GetComponentByClass<UForceApplicationComponent>();
		if (IsValid(ForceComp))
		{
			FVector Direction = IsValid(CenterComponent) ? (ColliderComponent->GetComponentLocation() - CenterComponent->GetComponentLocation()).GetSafeNormal() : ColliderComponent->GetRelativeLocation().GetSafeNormal();
			ForceComp->ApplyImpulse(Direction * HitPushbackStrength);
		}
	}
}

void UPushFromCenterAttackObject::StartPush()
{
	bPushing = true;
	PushTimer = 0.f;
	for (UPrimitiveComponent* Collider : Colliders)
	{
		if (IsValid(Collider))
		{
			Collider->SetCollisionProfileName(OverlapProfileName);
		}
	}
}

void UPushFromCenterAttackObject::CompleteAttack()
{
	OnAttackCompletedDelegate.Broadcast(this);
}
