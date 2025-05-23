// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthManager.h"
#include "BaseHealthComponent.h"
#include "Upgrades/TriggerHandlingComponent.h"
#include <Kismet\GameplayStatics.h>
#include "GameFramework/PlayerState.h" 

// Sets default values for this component's properties
UHealthManager::UHealthManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AActor* Owner = GetOwner();
	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthManager::TakeDamage);
	Owner->GetComponents<UBaseHealthComponent>(HealthComponents);
	ensure(HealthComponents.Num() > 0);
	HealthComponents.Sort([](const UBaseHealthComponent& first, const UBaseHealthComponent& second)
		{
			return first.GetPriority() > second.GetPriority();
		});

	for (UBaseHealthComponent* HealthComponent : HealthComponents)
	{
		//All damage is routed through the health manager
		Owner->OnTakeAnyDamage.RemoveAll(HealthComponent);
		HealthComponent->OnFatalDamageTakenDelegate.AddDynamic(this, &UHealthManager::OnHealthComponentFatalDamage);
	}
	HealthComponents.Sort([](const UBaseHealthComponent& first, const UBaseHealthComponent& second)
		{
			return first.GetPriority() > second.GetPriority();
		});
}

void UHealthManager::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	GetOwner()->OnTakeAnyDamage.RemoveDynamic(this, &UHealthManager::TakeDamage);
	for (UBaseHealthComponent* HealthComponent : HealthComponents)
	{
		HealthComponent->OnFatalDamageTakenDelegate.RemoveDynamic(this, &UHealthManager::OnHealthComponentFatalDamage);
	}
}

void UHealthManager::OnHealthComponentFatalDamage(UBaseHealthComponent* HealthComponent, float Damage, float OverkillDamage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	bool bAnyLiveHealthComponentsLeft = HealthComponents.ContainsByPredicate([](UBaseHealthComponent* HealthComponent)
		{
			return !HealthComponent->IsDepleted();
		});

	if (bAnyLiveHealthComponentsLeft)
	{
		if (HealthComponent->GetOverflows())
		{
			TakeDamage(HealthComponent->GetOwner(), OverkillDamage, DamageType, InstigatedBy, DamageCauser);
		}
	}
	else
	{
		if (IsValid(InstigatedBy))
		{
			APawn* Pawn = InstigatedBy->GetPawn();
			if (IsValid(Pawn))
			{
				UTriggerHandlingComponent* TriggerHandling = Pawn->GetComponentByClass<UTriggerHandlingComponent>();
				if (IsValid(TriggerHandling))
				{
					TriggerHandling->TriggerEnemyKilled(DamageCauser, GetOwner(), Damage);
				}
			}
		}
		OnFatalDamageTakenDelegate.Broadcast(HealthComponent, Damage, OverkillDamage, DamageType, InstigatedBy, DamageCauser);
	}
}

// Called every frame
void UHealthManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthManager::AddHealthComponent(UBaseHealthComponent* Component)
{
	Component->OnFatalDamageTakenDelegate.AddDynamic(this, &UHealthManager::OnHealthComponentFatalDamage);
	HealthComponents.Add(Component);

	HealthComponents.Sort([](const UBaseHealthComponent& first, const UBaseHealthComponent& second)
		{
			return first.GetPriority() > second.GetPriority();
		});
}

void UHealthManager::RemoveHealthComponent(UBaseHealthComponent* Component)
{
	Component->OnFatalDamageTakenDelegate.RemoveDynamic(this, &UHealthManager::OnHealthComponentFatalDamage);
	HealthComponents.Remove(Component);

	HealthComponents.Sort([](const UBaseHealthComponent& first, const UBaseHealthComponent& second)
		{
			return first.GetPriority() > second.GetPriority();
		});
}

void UHealthManager::SetInvulnerability(bool NewInvulnerability)
{
	bInvulnerable = NewInvulnerability;
}

int UHealthManager::GetLiveHealthComponentCount()
{
	return HealthComponents.FilterByPredicate([](const UBaseHealthComponent* HealthComponent) {return !HealthComponent->IsDepleted(); }).Num();
}

TArray<class UBaseHealthComponent*>& UHealthManager::GetHealthComponents()
{
	return HealthComponents;
}

float UHealthManager::GetTotalRemainingHealth()
{
	float RemainingHealth = 0;

	for (const UBaseHealthComponent* HealthComp : HealthComponents)
	{
		if (!HealthComp->IsDepleted())
		{
			RemainingHealth += HealthComp->GetCurrentHealth();
		}
	}

	return RemainingHealth;
}

float UHealthManager::GetTotalRemainingHealthPercentage()
{
	float RemainingHealth = 0.f;
	float TotalHealth = 0.f;

	for (const UBaseHealthComponent* HealthComp : HealthComponents)
	{
		if (!HealthComp->IsDepleted())
		{
			RemainingHealth += HealthComp->GetCurrentHealth();
			TotalHealth += HealthComp->GetMaxHealth();
		}
	}

	return RemainingHealth / TotalHealth;
}

void UHealthManager::ResetHealthComponents()
{
	for (UBaseHealthComponent* HealthComp : HealthComponents)
	{
		HealthComp->ResetHealth();
	}
}

void UHealthManager::Kill(const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	for (UBaseHealthComponent* HealthComp : HealthComponents)
	{
		HealthComp->Kill(DamageType, InstigatedBy, DamageCauser);
	}
}

void UHealthManager::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bInvulnerable)
		return;

	//Health components are kept sorted by priority
	for (size_t i = 0; i < HealthComponents.Num(); i++)
	{
		if (!HealthComponents[i]->IsDepleted())
		{
			OnDamageTakenDelegate.Broadcast(HealthComponents[i], Damage, DamageType, InstigatedBy, DamageCauser);
			HealthComponents[i]->TakeDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);

			if (IsValid(InstigatedBy))
			{
				APawn* Pawn = InstigatedBy->GetPawn();
				if (IsValid(Pawn))
				{
					UTriggerHandlingComponent* TriggerHandling = Pawn->GetComponentByClass<UTriggerHandlingComponent>();
					if (IsValid(TriggerHandling))
					{
						TriggerHandling->TriggerDamageDealt(DamageCauser, GetOwner(), Damage);
					}
				}
			}
			
			if (HealthComponents[i]->GetBlocksDamage())
			{
				return;
			}
		}
	}
}

void UHealthManager::Heal(float HealAmount)
{
	HealthComponents.Sort([](const UBaseHealthComponent& first, const UBaseHealthComponent& second)
		{
			return first.GetPriority() < second.GetPriority();
		});

	for (UBaseHealthComponent* HealthComp : HealthComponents)
	{
		if (HealthComp->CanBeHealed())
		{
			HealthComp->Heal(HealAmount);
			return;
		}
	}
}
