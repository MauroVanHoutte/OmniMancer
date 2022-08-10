// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WizardCharacter.h"
#include "OmnimancerGameMode.generated.h"

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
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWizardCharacter> FireWizardBlueprint;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWizardCharacter> FrostWizardBlueprint;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWizardCharacter> WindWizardBlueprint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> RoomSpawnPointBlueprint;
	TArray<AActor*> RoomSpawnPoints;
};
