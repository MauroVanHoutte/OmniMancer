// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnLocation.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include <Components/DecalComponent.h>

ASpawnLocation::ASpawnLocation()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetupAttachment(RootComponent);
	RootComponent = Root;
	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->SetupAttachment(RootComponent);
}

void ASpawnLocation::StartSpawnTimer(float Duration, TSubclassOf<AActor> ActorType, float Delay = 0)
{	
	TypeToSpawn = ActorType;
	if (Duration > 0)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		FTimerHandle Handle;
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &ASpawnLocation::SpawnEnemy);
		TimerManager.SetTimer(Handle, Delegate, Duration + Delay, false);

		if (Delay > 0)
		{
			FTimerHandle WarningHandle;
			FTimerDelegate WarningDelegate;
			Delegate.BindUObject(this, &ASpawnLocation::EnableWarning);
			TimerManager.SetTimer(WarningHandle, Delegate, Delay, false);
		}
		else
		{
			EnableWarning();
		}
	}
	else
	{
		SpawnEnemy();
	}
}

void ASpawnLocation::BeginPlay()
{
	Super::BeginPlay();

	Decal->SetVisibility(false);
	PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
}

void ASpawnLocation::EnableWarning()
{
	Decal->SetVisibility(true);
}

void ASpawnLocation::SpawnEnemy()
{
	if (IsValid(TypeToSpawn))
	{
		AActor* SpawnedEnemy = PoolingSystem->GetActorFromPool(TypeToSpawn);
		SpawnedEnemy->SetActorLocationAndRotation(GetActorLocation(), FRotator(0, 0, 0));
		OnEnemySpawnedDelegate.Broadcast(SpawnedEnemy);
	}
}
