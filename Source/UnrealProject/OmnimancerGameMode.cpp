// Fill out your copyright notice in the Description page of Project Settings.


#include "OmnimancerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "OmnimancerGameInstance.h"
#include "Enemies/EnemyPlacer.h"
#include "LevelGeneration/WaveFunctionCollapse.h"
#include "NavigationSystem.h" 

void AOmnimancerGameMode::BeginPlay()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	TArray<AActor*> wfcInstances;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveFunctionCollapse::StaticClass(), wfcInstances);
	if (wfcInstances.Num() > 0)
		WaveFunctionCollapse = Cast<AWaveFunctionCollapse>(wfcInstances[0]); //find level generator in scene

	//TODO create blueprint child of gamemode and add WaveFunctionCollapse and EnemyPlacer as instanced variables

	TArray<AActor*> enemyPlacerInstances;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyPlacer::StaticClass(), enemyPlacerInstances);
	if (enemyPlacerInstances.Num() > 0)
		EnemyPlacer = Cast<AEnemyPlacer>(enemyPlacerInstances[0]); //find enemy placer in scene

	SetupGeneratedLevel();

	TArray<AActor*> taggedActors; //find spawnpoint and spawn selected wizard type
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MainSpawnPoint"), taggedActors);
	if (taggedActors.Num() > 0)
	{
		SpawnPoint = taggedActors[0];
		auto gameInstance = GetGameInstance<UOmnimancerGameInstance>();
		WizardElement wizardType = gameInstance->GetSelectedType();

		AWizardCharacter* player = nullptr;

		switch (wizardType)
		{
		case WizardElement::Fire:
			player = GetWorld()->SpawnActor<AWizardCharacter>(FireWizardBlueprint.Get(), SpawnPoint->GetTransform());
			break;
		case WizardElement::Frost:
			player = GetWorld()->SpawnActor<AWizardCharacter>(FrostWizardBlueprint.Get(), SpawnPoint->GetTransform());
			break;
		case WizardElement::Wind:
			player = GetWorld()->SpawnActor<AWizardCharacter>(WindWizardBlueprint.Get(), SpawnPoint->GetTransform());
			break;
		default:
			break;
		}

		if (player != nullptr)
		{
			GetWorld()->GetFirstPlayerController()->Possess(player);
		}
	}
}

void AOmnimancerGameMode::NextRoom()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TArray<AActor*> characters; //Destroy leftover enemies
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), characters);
	for (AActor* actor : characters)
	{
		if (actor != player)
			actor->Destroy();
	}

	SetupGeneratedLevel();

	player->SetActorLocation(SpawnPoint->GetActorLocation());
}

void AOmnimancerGameMode::SetupGeneratedLevel()
{
	if (WaveFunctionCollapse == nullptr)
	{
		PostGenerationLevelSetup(nullptr); // no need to wait, navigation doesnt need rebuilding
		return;
	}
	UNavigationSystemV1* navigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	navigationSystem->OnNavigationGenerationFinishedDelegate.AddDynamic(this, &AOmnimancerGameMode::PostGenerationLevelSetup); // Enemy placer uses navmesh for enemy placement
	WaveFunctionCollapse->CreateLevel();
	navigationSystem->Build();
}

void AOmnimancerGameMode::PostGenerationLevelSetup(ANavigationData* data)
{
	if (EnemyPlacer != nullptr)
		EnemyPlacer->PlaceEnemies();
	UNavigationSystemV1* navigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	navigationSystem->OnNavigationGenerationFinishedDelegate.RemoveDynamic(this, &AOmnimancerGameMode::PostGenerationLevelSetup);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}
