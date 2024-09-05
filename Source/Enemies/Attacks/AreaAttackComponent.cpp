// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Attacks/AreaAttackComponent.h"
#include <AIController.h>
#include <Components/PrimitiveComponent.h>

UAreaAttackComponent::UAreaAttackComponent()
{
}

bool UAreaAttackComponent::AreAttackRequirementsMet()
{
	return FVector::DistSquared(GetOwner()->GetActorLocation(), Target->GetActorLocation()) < Range * Range;
}

void UAreaAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
		Target = PlayerController->GetPawn();

	TArray<UActorComponent*> TaggedComponents = GetOwner()->GetComponentsByTag(UPrimitiveComponent::StaticClass(), ColliderTag);
	for (UActorComponent* TaggedComponent : TaggedComponents)
	{
		UPrimitiveComponent* Collider = Cast<UPrimitiveComponent>(TaggedComponent);
		if (IsValid(Collider))
		{
			HurtBoxes.Add(Collider);
		}
	}
}

void UAreaAttackComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UAreaAttackComponent::InitiateAttack()
{
	for (UPrimitiveComponent* Comp : HurtBoxes)
	{
		Comp->SetVisibility(true);
	}

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UAreaAttackComponent::StartDamage);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, ChargeUpTime, false);
	GetOwner<APawn>()->GetController<AAIController>()->SetFocus(nullptr);
}

void UAreaAttackComponent::InterruptAttack()
{
	for (UPrimitiveComponent* Comp : HurtBoxes)
	{
		Comp->SetVisibility(false);
		Comp->SetCollisionProfileName(NoCollisionProfileName);
	}
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UAreaAttackComponent::StartDamage()
{
	for (UPrimitiveComponent* Comp : HurtBoxes)
	{
		Comp->SetCollisionProfileName(OverlapProfileName);
	}

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UAreaAttackComponent::EndDamage);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, ChargeUpTime, false);
	GetOwner<APawn>()->GetController<AAIController>()->SetFocus(nullptr);
}

void UAreaAttackComponent::EndDamage()
{
	for (UPrimitiveComponent* Comp : HurtBoxes)
	{
		Comp->SetVisibility(false);
		Comp->SetCollisionProfileName(NoCollisionProfileName);
	}
	AttackCompletedDelegate.Broadcast(this);
}
