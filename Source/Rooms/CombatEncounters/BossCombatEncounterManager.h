// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rooms/CombatEncounters/BaseCombatEncounterManager.h"
#include "BossCombatEncounterManager.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UBossCombatEncounterManager : public UBaseCombatEncounterManager
{
	GENERATED_BODY()
	
public:
	virtual void StartEncounter() override;

protected:
	UFUNCTION()
	void OnBossFatalDamage(UBaseHealthComponent* DamagedComponent, float Damage, float OverkillDamage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION()
	void OnBossDestroyed(AActor* Actor, EEndPlayReason::Type EndPlayReason);
	UFUNCTION()
	void OnBossSpawned(AActor* Boss);

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APawn>> BossOptions;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASpawnLocation> SpawnLocationClass;
};
