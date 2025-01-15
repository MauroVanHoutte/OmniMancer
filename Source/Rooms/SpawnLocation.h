// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "SpawnLocation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawnedSignature, AActor*, SpawnedEnemy);

UCLASS()
class UNREALPROJECT_API ASpawnLocation : public AActor
{
	GENERATED_BODY()

public:
	ASpawnLocation();
	void StartSpawnTimer(float Duration, TSubclassOf<AActor> ActorType, float Delay);

	UPROPERTY(BlueprintAssignable)
	FOnEnemySpawnedSignature OnEnemySpawnedDelegate;
	
protected:
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UDecalComponent* Decal;
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* Root;

private:
	void EnableWarning();
	void SpawnEnemy();
	UPROPERTY(Transient)
	TSubclassOf<AActor> TypeToSpawn;
};
