// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Attacks/ModularAttacks/ModularAttackEffect/ModularAttackEffect.h"
#include "Enemies/Attacks/ModularAttacks/ModularAttackTargetting/ModularAttackTargetting.h"

void UModularAttackEffectBase::OnBeginPlay(AActor* Owner)
{
	if (IsValid(TargettingModule)) TargettingModule->OnBeginPlay(Owner);
	OwningActor = Owner;
}

void UModularAttackEffectBase::OnEndPlay()
{
	if (IsValid(TargettingModule))
	{
		TargettingModule->OnEndPlay();
		TargettingModule->ConditionalBeginDestroy();
	}
}

void UModularAttackEffectBase::GatherTargets(const FVector& TargetLocation, TArray<AActor*>& outTargetActors, TArray<FVector>& outTargetLocations)
{
	if (IsValid(TargettingModule))
	{
		TargettingModule->GatherTargets(TargetLocation, outTargetActors, outTargetLocations);
	}
	else
	{
		outTargetLocations.Add(TargetLocation);
	}
}

void UModularAttackEffectBase::OnAttackCompleted()
{
	OnAttackCompletedDelegate.Broadcast(this);
}
