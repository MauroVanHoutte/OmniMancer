// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Attacks/AreaAttackObject.h"
#include <AIController.h>
#include <Components/PrimitiveComponent.h>
#include "Engine/DamageEvents.h"

void UAreaAttackObject::OnEndPlay()
{
	OwningActor->GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
	OwningActor->GetWorld()->GetTimerManager().ClearTimer(CompletionTimerHandle);
}

bool UAreaAttackObject::AreAttackRequirementsMet(AActor* Target)
{
	return FVector::DistSquared(OwningActor->GetActorLocation(), Target->GetActorLocation()) < Range * Range;
}

bool UAreaAttackObject::WasActorHitBefore(AActor* TriggeredActor)
{
	return HitActors.Contains(TriggeredActor);
}

void UAreaAttackObject::OnHitTriggered(AActor* HitActor)
{
	HitActors.Add(HitActor);
	FDamageEvent DamageEvent;
	HitActor->TakeDamage(Damage, DamageEvent, OwningActor->GetInstigatorController(), OwningActor);
}

void UAreaAttackObject::InitiateAttack(AActor* Target)
{
	Super::InitiateAttack(Target);

	HitActors.Empty();

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

	for (UPrimitiveComponent* Comp : HurtBoxes)
	{
		Comp->SetHiddenInGame(false);
	}

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UAreaAttackObject::StartDamage);
	GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, Delegate, ChargeUpTime, false);
	Cast<APawn>(OwningActor)->GetController<AAIController>()->SetFocus(nullptr);
}

void UAreaAttackObject::InterruptAttack()
{
	Super::InterruptAttack();

	for (UPrimitiveComponent* Comp : HurtBoxes)
	{
		Comp->SetHiddenInGame(true);
		Comp->SetCollisionProfileName(NoCollisionProfileName);
	}
	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(CompletionTimerHandle);
}

void UAreaAttackObject::StartDamage()
{
	for (UPrimitiveComponent* Comp : HurtBoxes)
	{
		Comp->SetCollisionProfileName(OverlapProfileName);
	}

	FTimerDelegate DamageDelegate;
	DamageDelegate.BindUObject(this, &UAreaAttackObject::EndDamage);
	GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, DamageDelegate, ActiveTime, false);
	
	FTimerDelegate CompletionDelegate;
	CompletionDelegate.BindUObject(this, &UAreaAttackObject::AttackCompleted);
	GetWorld()->GetTimerManager().SetTimer(CompletionTimerHandle, CompletionDelegate, AttackCompletionDelay, false);

	Cast<APawn>(OwningActor)->GetController<AAIController>()->SetFocus(nullptr);
}

void UAreaAttackObject::EndDamage()
{
	for (UPrimitiveComponent* Comp : HurtBoxes)
	{
		Comp->SetHiddenInGame(true);
		Comp->SetCollisionProfileName(NoCollisionProfileName);
	}
}

void UAreaAttackObject::AttackCompleted()
{
	OnAttackComponentCompletedDelegate.Broadcast(this);
}