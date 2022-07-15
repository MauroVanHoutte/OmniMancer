// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OmnimancerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UOmnimancerSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY();
	int Currency = 0;
	UPROPERTY();
	int FireUpgrades = 0;
	UPROPERTY();
	int FrostUpgrades = 0;
	UPROPERTY();
	int WindUpgrades = 0;
};
