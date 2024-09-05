// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeAttackComponent.h"
#include <AIController.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>

UChargeAttackComponent::UChargeAttackComponent()
{
}

void UChargeAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

		GetOwner()->AddActorLocalOffset(FVector(DistanceToMove, 0, 0), true);

		if (!bCharging)
		{
			Collider->SetCollisionProfileName(NoCollisionProfileName);
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &UChargeAttackComponent::CooldownCompleted);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, CooldownTime, false);
		}
	}
}

bool UChargeAttackComponent::AreAttackRequirementsMet()
{
	float DistSquared = FVector::DistSquared(GetOwner()->GetActorLocation(), Target->GetActorLocation());
	if (DistSquared < Range * Range)
	{
		FHitResult Hit;
		TArray<AActor*> IgnoreActors;
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
			GetOwner()->GetActorLocation(),
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

void UChargeAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
		Target = PlayerController->GetPawn();

	TArray<UActorComponent*> TaggedComponents = GetOwner()->GetComponentsByTag(USceneComponent::StaticClass(), ColliderAttachmentTag);
	USceneComponent* TaggedComponent = TaggedComponents.Num() > 0 ? Cast<USceneComponent>(TaggedComponents[0]) : nullptr;

	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	Collider->AttachToComponent(TaggedComponent ? TaggedComponent : GetOwner()->GetRootComponent(), Rules);
}

void UChargeAttackComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UChargeAttackComponent::InitiateAttack()
{
	FVector TargetLocation = Target->GetActorLocation();
	TargetLocation.Z = 0;
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	OwnerLocation.Z = 0;
	GetOwner()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(OwnerLocation, TargetLocation));
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UChargeAttackComponent::StartCharge);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, ChargeUpTime, false);
	GetOwner<APawn>()->GetController<AAIController>()->SetFocus(nullptr);
}

void UChargeAttackComponent::InterruptAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	bCharging = false;
}

void UChargeAttackComponent::StartCharge()
{
	bCharging = true;
	ChargedDistance = 0.f;
	Collider->SetCollisionProfileName(OverlapProfileName);
}

void UChargeAttackComponent::CooldownCompleted()
{
	GetOwner<APawn>()->GetController<AAIController>()->SetFocus(Target);
	AttackCompletedDelegate.Broadcast(this);
}
