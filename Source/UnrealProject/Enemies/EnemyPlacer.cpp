// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyPlacer.h"
#include "Kismet/GameplayStatics.h" 
#include "NavigationSystem.h"

// Sets default values
AEnemyPlacer::AEnemyPlacer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyPlacer::PlaceEnemies()
{
	TArray<AActor*> tileActors{};
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Tile"), tileActors);
	for (AActor* tile : tileActors)
	{
		bool isEnemyRoom = FMath::FRand() <= EnemyRoomChance; // to have some rooms without enemies
		if (isEnemyRoom && (StartTarget == nullptr || FVector::DistSquared(tile->GetActorLocation(), StartTarget->GetActorLocation()) > SpawnSafeRadius*SpawnSafeRadius)) // tiles too close to spawn dont have enemies
		{
			FVector origin;
			FVector extent;
			tile->GetActorBounds(true, origin, extent);
			float spawnRadius = extent.X / 2;
			auto navigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
			for (size_t i = 0; i < PacksPerTile; i++)
			{
				FNavLocation chosenPackLocation;
				navigationSystem->GetRandomReachablePointInRadius(origin, spawnRadius, chosenPackLocation);
				int packSize = FMath::RandRange(MinPackSize, MaxPackSize);

				//weightedRandom
				TSubclassOf<ABaseCharacter> enemyType;
				float totalWeight = 0;
				for (auto pair : Enemies)
					totalWeight += pair.Value;

				float rand = FMath::FRandRange(0, totalWeight);

				for (auto pair : Enemies)
				{
					rand -= pair.Value;
					if (rand < 0)
					{
						enemyType = pair.Key;
						break;
					}
				}
 
				for (size_t j = 0; j < packSize; j++)
				{
					FNavLocation enemySpawnLocation;
					navigationSystem->GetRandomReachablePointInRadius(chosenPackLocation.Location, PackRadius, enemySpawnLocation); // random spots around pack spawn point
					FActorSpawnParameters spawnParams{};
					spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
					GetWorld()->SpawnActor<AActor>(enemyType.Get(), enemySpawnLocation.Location, FRotator(0, FMath::FRand(), 0), spawnParams);
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AEnemyPlacer::BeginPlay()
{
	Super::BeginPlay();
	PlaceEnemies();
}

// Called every frame
void AEnemyPlacer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

