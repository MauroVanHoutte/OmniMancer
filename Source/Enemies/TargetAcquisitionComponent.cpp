// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/TargetAcquisitionComponent.h"
#include "Algo/MinElement.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "Health/AffiliationComponent.h"
#include <Kismet/GameplayStatics.h>

void UTargetAcquisitionComponent::Initialize(AAIController* Controller, class UAffiliationComponent* Affiliation)
{
	OwningController = Controller;
	AffiliationComponent = Affiliation;
}

void UTargetAcquisitionComponent::AcquireTarget()
{
	//Only Actors with different affiliation can be targets
	//If affiliation is missing target is also valid
	TArray<FOverlapInfo> NonAlliedOverlappingComponents = OverlappingComponents.FilterByPredicate([this](const FOverlapInfo& OverlapInfo)
		{
			UAffiliationComponent* PotentialTargetAffiliation = OverlapInfo.OverlapInfo.GetActor()->GetComponentByClass<UAffiliationComponent>();
			return (IsValid(AffiliationComponent) && IsValid(PotentialTargetAffiliation)) ? AffiliationComponent->GetAffiliation() != PotentialTargetAffiliation->GetAffiliation() : true;
		});

	TArray<AActor*> ValidTargets;
	for (const FOverlapInfo& OverlapInfo : NonAlliedOverlappingComponents)
	{
		ValidTargets.Add(OverlapInfo.OverlapInfo.GetActor());
	}

	//If no targets check in overlapping components check all actors
	if (ValidTargets.IsEmpty())
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), Actors);
		ValidTargets = Actors.FilterByPredicate([this](AActor* Actor)
			{
				UAffiliationComponent* PotentialTargetAffiliation = Actor->GetComponentByClass<UAffiliationComponent>();
				return (IsValid(AffiliationComponent) && IsValid(PotentialTargetAffiliation)) ? AffiliationComponent->GetAffiliation() != PotentialTargetAffiliation->GetAffiliation() : true;
			});
	}

	if (!ValidTargets.IsEmpty())
	{
		AActor* NewTarget = *Algo::MinElementBy(ValidTargets, [this](const AActor* Target)
			{
				return FVector::DistSquared(Target->GetActorLocation(), GetComponentLocation());
			});

		if (IsValid(OwningController))
		{
			UBlackboardComponent* Blackboard = OwningController->GetBlackboardComponent();
			if (IsValid(Blackboard))
				Blackboard->SetValueAsObject(TargetBlackboardKey, NewTarget);
		}
	}
}
