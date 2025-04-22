// Fill out your copyright notice in the Description page of Project Settings.

#include "BossCombatEncounterManager.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include "Health/HealthManager.h"
#include "Rooms/SpawnLocation.h"

void UBossCombatEncounterManager::StartEncounter()
{
	Super::StartEncounter();

	if (!BossOptions.IsEmpty())
	{
		UActorPoolingSubsystem* PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
		TSubclassOf<APawn> Chosen = BossOptions[FMath::RandRange(0, BossOptions.Num() - 1)];
		if (IsValid(SpawnLocationClass))
		{
			ASpawnLocation* BossSpawnLocation = Cast<ASpawnLocation>(PoolingSystem->GetActorFromPool(SpawnLocationClass));
			BossSpawnLocation->SetActorLocationAndRotation(GetComponentLocation(), FRotator(0, 0, 0));
			BossSpawnLocation->StartSpawnTimer(4.f, Chosen, 0.5f);
			BossSpawnLocation->OnEnemySpawnedDelegate.AddDynamic(this, &UBossCombatEncounterManager::OnBossSpawned);
		}
		else
		{
			AActor* Boss = PoolingSystem->GetActorFromPool(Chosen);
			Boss->SetActorLocationAndRotation(GetComponentLocation(), FRotator(0, 0, 0));

			UHealthManager* HealthManager = Boss->GetComponentByClass<UHealthManager>();
			if (IsValid(HealthManager))
			{
				HealthManager->OnFatalDamageTakenDelegate.AddDynamic(this, &UBossCombatEncounterManager::OnBossFatalDamage);
			}
			else
			{
				Boss->OnEndPlay.AddDynamic(this, &UBossCombatEncounterManager::OnBossDestroyed);
			}
		}
	}
	else
	{
		OnBossDestroyed(nullptr, EEndPlayReason::Destroyed);
	}
}

void UBossCombatEncounterManager::OnBossFatalDamage(UBaseHealthComponent* DamagedComponent, float Damage, float OverkillDamage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	EncounterCompleted();
}

void UBossCombatEncounterManager::OnBossDestroyed(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	EncounterCompleted();
}

void UBossCombatEncounterManager::OnBossSpawned(AActor* Boss)
{
	UHealthManager* HealthManager = Boss->GetComponentByClass<UHealthManager>();
	if (IsValid(HealthManager))
	{
		HealthManager->OnFatalDamageTakenDelegate.AddDynamic(this, &UBossCombatEncounterManager::OnBossFatalDamage);
	}
	else
	{
		Boss->OnEndPlay.AddDynamic(this, &UBossCombatEncounterManager::OnBossDestroyed);
	}
}
