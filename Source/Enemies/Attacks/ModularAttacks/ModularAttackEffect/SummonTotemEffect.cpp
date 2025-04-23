// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/Attacks/ModularAttacks/ModularAttackEffect/SummonTotemEffect.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include "Health/HealthManager.h"
#include "Health/AffiliationComponent.h"
#include "NavigationSystem.h"

void USummonTotemEffect::OnEndPlay()
{
	Super::OnEndPlay();

	if (bKillTotemsOnCasterDeath)
	{
		for (AActor* Totem : SpawnedTotems)
		{
			if (IsValid(Totem))
			{
				UHealthManager* HealthManager = Totem->GetComponentByClass<UHealthManager>();
				if (IsValid(HealthManager))
				{
					HealthManager->Kill(nullptr, nullptr, nullptr);
				}
				else
				{
					UActorPoolingSubsystem* PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
					if (IsValid(PoolingSystem))
						PoolingSystem->ReturnActorToPoolOrDestroy(Totem);
					else
						Totem->Destroy();
				}
			}
		}
	}
}

void USummonTotemEffect::ExecuteEffect(AActor* TargetActor, const FVector& TargetLocation)
{
	TargetLocations.Empty();
	TArray<AActor*> Actors;
	GatherTargets(TargetLocation, Actors, TargetLocations);

	if (ChargeTime > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &USummonTotemEffect::SummonTotems);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, ChargeTime, false);
	}
	else
	{
		SummonTotems();
	}
}

void USummonTotemEffect::SummonTotems()
{
	if (IsValid(TotemClass))
	{
		for (const FVector& Location : TargetLocations)
		{
			UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
			FNavLocation NavLocation;
			if (NavSystem->ProjectPointToNavigation(Location, NavLocation, FVector(1000, 1000, 1000)))
			{
				UActorPoolingSubsystem* PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
				AActor* NewTotem = PoolingSystem->GetActorFromPool(TotemClass);
				NewTotem->SetOwner(OwningActor);
				NewTotem->SetInstigator(Cast<APawn>(OwningActor));
				if (IsValid(NewTotem))
				{
					NewTotem->SetActorLocationAndRotation(NavLocation.Location, FRotator(0, 0, 0));
					NewTotem->SetLifeSpan(TotemLifeTime);
					UAffiliationComponent* Affiliation = NewTotem->GetComponentByClass<UAffiliationComponent>();
					UAffiliationComponent* OwnerAffiliation = OwningActor->GetComponentByClass<UAffiliationComponent>();

					if (IsValid(Affiliation) && IsValid(OwnerAffiliation))
					{
						Affiliation->SetAffiliation(OwnerAffiliation->GetAffiliation());
					}

					SpawnedTotems.Add(NewTotem);
				}
			}
		}
	}

	if (CooldownTime > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UModularAttackEffectBase::OnAttackCompleted);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, CooldownTime, false);
	}
	else
	{
		OnAttackCompleted();
	}
}
