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
		bool isEnemyRoom = FMath::FRand() <= EnemyRoomChance;
		if (isEnemyRoom && (StartTarget == nullptr || FVector::DistSquared(tile->GetActorLocation(), StartTarget->GetActorLocation()) > SpawnSafeRadius*SpawnSafeRadius))
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
				TSubclassOf<ABaseCharacter> enemyType = Enemies[FMath::RandRange(0, Enemies.Num() - 1)];
				for (size_t j = 0; j < packSize; j++)
				{
					FNavLocation enemySpawnLocation;
					navigationSystem->GetRandomReachablePointInRadius(chosenPackLocation.Location, PackRadius, enemySpawnLocation);
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

