// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WizardCharacter.h"
#include "OmnimancerGameMode.generated.h"

class AEnemyPlacer;
class AWaveFunctionCollapse;
class ANavigationData;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AOmnimancerGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void NextRoom();

private:

	void SetupGeneratedLevel();
	UFUNCTION()
	void PostGenerationLevelSetup(ANavigationData* data);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> FireWizardBlueprint;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> FrostWizardBlueprint;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> WindWizardBlueprint;

	AEnemyPlacer* EnemyPlacer = nullptr;
	AWaveFunctionCollapse* WaveFunctionCollapse = nullptr;
	AActor* SpawnPoint = nullptr;
};
