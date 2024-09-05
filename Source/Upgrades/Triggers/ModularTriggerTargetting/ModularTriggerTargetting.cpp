// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularTriggerTargetting.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Health/AffiliationComponent.h"

void UBPModularTriggerTargettingBase::GatherTargets(AActor* inTriggerOwner, ABaseSpell* inSpell, AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<AActor*>& outTargetActors)
{
	BP_GatherTargets(inTriggerOwner, inSpell, inTarget, outTargetLocations, outTargetActors);
}

void UTargetHitActor::GatherTargets(AActor* inTriggerOwner, ABaseSpell* inSpell, AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<AActor*>& outTargetActors)
{
	outTargetActors.Add(inTarget);
}

void UTargetHitActorLocation::GatherTargets(AActor* inTriggerOwner, ABaseSpell* inSpell, AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<AActor*>& outTargetActors)
{
	outTargetLocations.Add(inTarget->GetActorLocation());
}

void UTargetActorsProximateToTarget::GatherTargets(AActor* inTriggerOwner, ABaseSpell* inSpell, AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<AActor*>& outTargetActors)
{
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), inTarget->GetActorLocation(), Radius, {}, AActor::StaticClass(), {}, outTargetActors);

	if (bCheckAffiliation)
	{
		UAffiliationComponent* TriggerAffiliation = inTriggerOwner->GetComponentByClass<UAffiliationComponent>();

		outTargetActors.RemoveAll([TriggerAffiliation](AActor* Actor)
			{
				UAffiliationComponent* ActorAffiliation = Actor->GetComponentByClass<UAffiliationComponent>();
				return TriggerAffiliation && ActorAffiliation && ActorAffiliation->GetAffiliation() == TriggerAffiliation->GetAffiliation();
			});
	}
}

void UTargetActorsProximateToOwner::GatherTargets(AActor* inTriggerOwner, ABaseSpell* inSpell, AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors)
{
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), inTriggerOwner->GetActorLocation(), Radius, {}, AActor::StaticClass(), {}, outTargetActors);

	if (bCheckAffiliation)
	{
		UAffiliationComponent* TriggerAffiliation = inTriggerOwner->GetComponentByClass<UAffiliationComponent>();

		outTargetActors.RemoveAll([TriggerAffiliation](AActor* Actor)
			{
				UAffiliationComponent* ActorAffiliation = Actor->GetComponentByClass<UAffiliationComponent>();
				return TriggerAffiliation && ActorAffiliation && ActorAffiliation->GetAffiliation() == TriggerAffiliation->GetAffiliation();
			});
	}
}
