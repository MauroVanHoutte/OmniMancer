// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCharacter.h"
#include "EnemyPlacer.generated.h"

UCLASS()
class UNREALPROJECT_API AEnemyPlacer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyPlacer();

	void PlaceEnemies();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	AActor* StartTarget;
	UPROPERTY(EditAnywhere)
	float SpawnSafeRadius = 3000.f;
	UPROPERTY(EditAnywhere)
	AActor* EndTarget;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseCharacter>> Enemies;
	UPROPERTY(EditAnywhere)
	int PacksPerTile = 3;
	UPROPERTY(EditAnywhere)
	int MinPackSize = 1;
	UPROPERTY(EditAnywhere)
	int MaxPackSize = 3;
	UPROPERTY(EditAnywhere)
	float PackRadius = 300.f;
};
