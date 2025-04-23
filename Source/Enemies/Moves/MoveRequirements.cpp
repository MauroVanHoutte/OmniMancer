// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Moves/MoveRequirements.h"
#include "Health/HealthManager.h"

bool UHealthMoveRequirement::AreRequirementsMet(AActor* Owner, AActor* Target)
{
	UHealthManager* ComponentToTest = bCheckOwnerHealth ? Owner->GetComponentByClass<UHealthManager>() : Target->GetComponentByClass<UHealthManager>();
	float HealthTestValue = bUsePercentageThreshold ? ComponentToTest->GetTotalRemainingHealthPercentage() : ComponentToTest->GetTotalRemainingHealth();
	return bRequireHealthBelowThreshold ? HealthTestValue < Threshold : HealthTestValue > Threshold;
}

bool ULimitedUsesRequirement::AreRequirementsMet(AActor* Owner, AActor* Target)
{
	return UseCounter < NumberOfUses;
}

void ULimitedUsesRequirement::OnAttackExecuted(AActor* Owner, AActor* Target)
{
	UseCounter++;
}

bool URangeRequirement::AreRequirementsMet(AActor* Owner, AActor* Target)
{
	float SqrDistance = FVector::DistSquared(Owner->GetTargetLocation(), Target->GetActorLocation());
	return bRequireCloserThanThreshold ? SqrDistance < RangeThreshold * RangeThreshold : SqrDistance > RangeThreshold * RangeThreshold;
}

bool UCooldownRequirement::AreRequirementsMet(AActor* Owner, AActor* Target)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	return !TimerManager.IsTimerActive(TimerHandle);
}

void UCooldownRequirement::OnAttackExecuted(AActor* Owner, AActor* Target)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, CooldownDuration, false);
}

bool USphereTraceRequirement::AreRequirementsMet(AActor* Owner, AActor* Target)
{
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Owner);
	IgnoreActors.Add(Target);
	FVector Direction = (Target->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal();
	FHitResult Hit;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
		Owner->GetActorLocation(),
		Owner->GetActorLocation() + (Direction * TraceLength),
		TraceRadius,
		UEngineTypes::ConvertToTraceType(TraceChannel),
		true,
		IgnoreActors,
		DebugDraw,
		Hit,
		true,
		FLinearColor::Green,
		FLinearColor::Red,
		DebugDrawDuration);

	return !Hit.bBlockingHit;
}
