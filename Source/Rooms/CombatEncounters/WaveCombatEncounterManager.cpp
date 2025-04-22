// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveCombatEncounterManager.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include <Components/ShapeComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <NavigationSystem.h>
#include "Rooms/SpawnLocation.h"

UWaveCombatEncounterManager::UWaveCombatEncounterManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);
}

void UWaveCombatEncounterManager::StartEncounter()
{
	Super::StartEncounter();

	if (PossibleEncounters.Num() > 0)
	{
		SelectedEncounterIndex = FMath::RandRange(0, PossibleEncounters.Num() - 1);
		if (IsValid(PossibleEncounters[SelectedEncounterIndex]))
		{
			StartWaves();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Encounter at index %d on %s is invalid"), SelectedEncounterIndex, *GetOwner()->GetActorNameOrLabel());
		}
	}
}

void UWaveCombatEncounterManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (size_t i = 0; i < SpawnedEnemies.Num(); i++)
	{
		if (IsValid(SpawnedEnemies[i]))
		{
			GEngine->AddOnScreenDebugMessage(i, 1.f, FColor::Cyan, FString::Printf(TEXT("%d %s"), i, *SpawnedEnemies[i]->GetActorNameOrLabel()));
		}
	}
}

void UWaveCombatEncounterManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);
	
	for (USceneComponent* Child : Children)
	{
		if (UShapeComponent* Shape = Cast<UShapeComponent>(Child))
		{
			SpawnZones.Add(Shape);
		}
	}
}

void UWaveCombatEncounterManager::StartWaves()
{
	CurrentWave = 0;
	SpawnWave();
}

void UWaveCombatEncounterManager::SpawnWave()
{
	ResetUnusedZones();
	UEncounterParams* Encounter = PossibleEncounters[SelectedEncounterIndex];
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TArray<UPackSpawnParams*> PacksToSpawn;
	if (Encounter->bSpecifiedWaves)
	{
		PacksToSpawn = Encounter->WavePacks[CurrentWave]->Packs;
	}
	else
	{
		int NrOfPacks = Encounter->bUseStaticPackCountPerWave ? Encounter->StaticPacksPerWave : Encounter->PacksPerWave[CurrentWave];
		for (size_t i = 0; i < NrOfPacks; i++)
		{
			int PackOptions = Encounter->RandomPackOptions.Num();
			PacksToSpawn.Add(Encounter->RandomPackOptions[FMath::RandRange(0, PackOptions - 1)]);
		}
	}

	int TotalWaves = Encounter->bSpecifiedWaves ? Encounter->WavePacks.Num() - 1 : Encounter->NrOfWaves;
	if (CurrentWave < TotalWaves)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UWaveCombatEncounterManager::SpawnWave);
		FTimerHandle Handle;

		float TimerDuration = Encounter->bUseStaticPackCountPerWave ? Encounter->StaticTimeBetweenWaves : Encounter->TimeBetweenWaves[CurrentWave];
		TimerManager.SetTimer(Handle, Delegate, TimerDuration, false);
	}

	for (const UPackSpawnParams* PackParams : PacksToSpawn)
	{
		QueuePackSpawn(FindPackSpawnLocation(), PackParams);
	}
	CurrentWave++;
}

FVector UWaveCombatEncounterManager::FindPackSpawnLocation()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FVector PackSpawnLocation;
	UShapeComponent* SpawnArea = PickSpawnZone();
	if (!IsValid(SpawnArea))
	{
		PackSpawnLocation = GetComponentLocation();
	}
	else
	{
		FCollisionShape Shape = SpawnArea->GetCollisionShape();
		switch (Shape.ShapeType)
		{
		case ECollisionShape::Box:
		{
			FVector Extents = Shape.GetBox();
			PackSpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->GetComponentLocation(), Extents / 2);
			break;
		}
		case ECollisionShape::Sphere:
		{
			float Radius = Shape.GetSphereRadius();
			FNavLocation NavLocation;
			NavSystem->GetRandomPointInNavigableRadius(SpawnArea->GetComponentLocation(), Radius, NavLocation);
			PackSpawnLocation = NavLocation.Location;
			break;
		}
		default:
			break;
		}
	}

	FNavLocation ProjectedLocation;
	NavSystem->ProjectPointToNavigation(PackSpawnLocation, ProjectedLocation, FVector(10000.f));
	return ProjectedLocation.Location;
}

void UWaveCombatEncounterManager::QueuePackSpawn(const FVector& PackCenter, const UPackSpawnParams* SpawnParams)
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	UActorPoolingSubsystem* PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
	if (IsValid(PoolingSystem))
	{
		for (const UEnemySpawnParams* EnemyParams : SpawnParams->PackComposition)
		{
			FNavLocation NavLocation;
			NavSystem->GetRandomReachablePointInRadius(PackCenter, SpawnParams->PackRadius, NavLocation);
			ASpawnLocation* SpawnWarning = Cast<ASpawnLocation>(PoolingSystem->GetActorFromPool(EnemyParams->SpawnIndicatorClass));
			SpawnWarning->SetActorLocationAndRotation(NavLocation.Location, FRotator(0, 0, 0));
			float Delay = EnemyParams->BaseDelay + FMath::RandRange(0.f, EnemyParams->DelayVariance);
			float Duration = EnemyParams->BaseWarningTime + FMath::RandRange(0.f, EnemyParams->WarningTimeVariance);
			TrackSpawnedEnemy(SpawnWarning);
			SpawnWarning->OnEnemySpawnedDelegate.AddUniqueDynamic(this, &UWaveCombatEncounterManager::TrackSpawnedEnemy);
			SpawnWarning->StartSpawnTimer(Duration, EnemyParams->EnemyClass, Delay);
		}
	}
}

UShapeComponent* UWaveCombatEncounterManager::PickSpawnZone()
{
	if (SpawnZones.IsEmpty())
	{
		return nullptr;
	}

	if (UnusedZones.IsEmpty())
	{
		ResetUnusedZones();
	}

	//Get zone player is currently overlapping or is closest first
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	TArray<UShapeComponent*> OverlappingZones = UnusedZones.FilterByPredicate([Player](const UShapeComponent* Shape)
		{
			return Shape->IsOverlappingActor(Player);
		});

	if (!OverlappingZones.IsEmpty())
	{
		UnusedZones.Remove(OverlappingZones[0]);
		return OverlappingZones[0];
	}
	else
	{
		UShapeComponent* ClosestZone = nullptr;
		float ClosestDistanceSquared = FLT_MAX;
		for (UShapeComponent* Zone : UnusedZones)
		{
			float DistanceToPlayer = FLT_MAX;
			FCollisionShape Shape = Zone->GetCollisionShape();

			//Calculate distance to player
			switch (Shape.ShapeType)
			{
			case ECollisionShape::Box:
			{
				//https://stackoverflow.com/questions/5254838/calculating-distance-between-a-point-and-a-rectangular-box-nearest-point
				FVector Min = Zone->GetComponentLocation() - FVector(Shape.Box.HalfExtentX, Shape.Box.HalfExtentY, Shape.Box.HalfExtentZ);
				FVector Max = Zone->GetComponentLocation() + FVector(Shape.Box.HalfExtentX, Shape.Box.HalfExtentY, Shape.Box.HalfExtentZ);
				FVector PlayerLocation = Player->GetActorLocation();
				float dX = FMath::Max(FMath::Max(Min.X - PlayerLocation.X, 0.0), PlayerLocation.X - Max.X);
				float dY = FMath::Max(FMath::Max(Min.Y - PlayerLocation.Y, 0.0), PlayerLocation.Y - Max.Y);
				float dZ = FMath::Max(FMath::Max(Min.Z - PlayerLocation.Z, 0.0), PlayerLocation.Z - Max.Z);
				DistanceToPlayer = FMath::Sqrt(dX * dX + dY * dY + dZ * dZ);
				break;
			}
			case ECollisionShape::Sphere:
			{
				DistanceToPlayer = FVector::Dist(Zone->GetComponentLocation(), Player->GetActorLocation()) - Shape.Sphere.Radius;
				break;
			}
			default:
				break;
			}

			if (DistanceToPlayer < ClosestDistanceSquared)
			{
				ClosestZone = Zone;
				ClosestDistanceSquared = DistanceToPlayer;
			}
		}

		UnusedZones.Remove(ClosestZone);
		return ClosestZone;
	}
}

void UWaveCombatEncounterManager::ResetUnusedZones()
{
	UnusedZones = SpawnZones;
}

void UWaveCombatEncounterManager::TrackSpawnedEnemy(AActor* SpawnedActor)
{
	if (IsValid(SpawnedActor))
	{
		if (IPooledActorInterface* PoolingInterface = Cast<IPooledActorInterface>(SpawnedActor))
		{
			PoolingInterface->GetReturnedToPoolDelegate().AddUniqueDynamic(this, &UWaveCombatEncounterManager::OnTrackedEnemyDestroyed);
		}
		else
		{
			SpawnedActor->OnDestroyed.AddUniqueDynamic(this, &UWaveCombatEncounterManager::OnTrackedEnemyDestroyed);
		}

		SpawnedEnemies.Add(SpawnedActor);
	}
}

void UWaveCombatEncounterManager::OnTrackedEnemyDestroyed(AActor* Actor)
{
	if (IPooledActorInterface* PoolingInterface = Cast<IPooledActorInterface>(Actor))
	{
		PoolingInterface->GetReturnedToPoolDelegate().RemoveDynamic(this, &UWaveCombatEncounterManager::OnTrackedEnemyDestroyed);
	}
	else
	{
		Actor->OnDestroyed.RemoveDynamic(this, &UWaveCombatEncounterManager::OnTrackedEnemyDestroyed);
	}

	SpawnedEnemies.Remove(Actor);
	UEncounterParams* Encounter = PossibleEncounters[SelectedEncounterIndex];
	int TotalWaves = Encounter->bSpecifiedWaves ? Encounter->WavePacks.Num() - 1 : Encounter->NrOfWaves;
	if (CurrentWave > TotalWaves && SpawnedEnemies.Num() == 0)
	{
		EncounterCompleted();
	}
}
