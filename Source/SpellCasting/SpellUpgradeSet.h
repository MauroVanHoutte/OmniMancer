// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpellUpgradeSet.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class UNREALPROJECT_API USpellUpgradeSet : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<class USpellUpgradeData*> AvailableUpgrades;
};
