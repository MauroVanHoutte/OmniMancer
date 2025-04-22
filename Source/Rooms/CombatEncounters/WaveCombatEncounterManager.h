// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rooms/CombatEncounters/BaseCombatEncounterManager.h"
#include "WaveCombatEncounterManager.generated.h"

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class UEnemySpawnParams : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float BaseDelay = 0;
	UPROPERTY(EditAnywhere)
	float DelayVariance = 0.2f;
	UPROPERTY(EditAnywhere)
	float BaseWarningTime = 1.5f;
	UPROPERTY(EditAnywhere)
	float WarningTimeVariance = 0.4f;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> EnemyClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASpawnLocation> SpawnIndicatorClass;
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class UPackSpawnParams : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float PackRadius;
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UEnemySpawnParams*> PackComposition;
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class UWavePacks : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UPackSpawnParams*> Packs;
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class UEncounterParams : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	bool bSpecifiedWaves = false;
	UPROPERTY(EditAnywhere)
	bool bUseStaticTimeBetweenWaves = true;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bSpecifiedWaves"))
	bool bUseStaticPackCountPerWave = true;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bSpecifiedWaves"))
	int NrOfWaves = 5;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bSpecifiedWaves && bUseStaticPackCountPerWave"))
	int StaticPacksPerWave = 4;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bSpecifiedWaves && !bUseStaticPackCountPerWave"))
	TArray<int> PacksPerWave;
	UPROPERTY(EditAnywhere, Instanced, meta = (EditCondition = "bSpecifiedWaves"))
	TArray<UWavePacks*> WavePacks;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUseStaticTimeBetweenWaves"))
	TArray<float> TimeBetweenWaves;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseStaticTimeBetweenWaves"))
	float StaticTimeBetweenWaves = 10.f;
	UPROPERTY(EditAnywhere, Instanced, meta = (EditCondition = "!bSpecifiedWaves"))
	TArray<UPackSpawnParams*> RandomPackOptions;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UWaveCombatEncounterManager : public UBaseCombatEncounterManager
{
	GENERATED_BODY()

public:
	UWaveCombatEncounterManager();
	
	virtual void StartEncounter() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	void BeginPlay() override;

	virtual void StartWaves();
	virtual void SpawnWave();
	virtual FVector FindPackSpawnLocation();
	virtual void QueuePackSpawn(const FVector& PackCenter, const UPackSpawnParams* SpawnParams);
	class UShapeComponent* PickSpawnZone();
	void ResetUnusedZones();
	UFUNCTION()
	virtual void TrackSpawnedEnemy(AActor* SpawnedActor);
	UFUNCTION()
	virtual void OnTrackedEnemyDestroyed(AActor* Actor);
private:
	UPROPERTY(Transient)
	TArray<class UShapeComponent*> SpawnZones;
	UPROPERTY(Transient)
	TArray<class UShapeComponent*> UnusedZones;
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UEncounterParams*> PossibleEncounters;
	UPROPERTY(Transient)
	TArray<AActor*> SpawnedEnemies;
	int SelectedEncounterIndex = 0;
	int CurrentWave = 0;
};
