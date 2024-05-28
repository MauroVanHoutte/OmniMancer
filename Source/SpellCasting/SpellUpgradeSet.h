// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpellUpgradeSet.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, EditInlineNew)
class USpellUpgrade : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBaseSpellUpgrade* Upgrade;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class ABaseSpell>> ApplicableSpells;
	UPROPERTY(EditDefaultsOnly)
	int Limit = 1;
};

UCLASS()
class UNREALPROJECT_API USpellUpgradeSet : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<USpellUpgrade*> AvailableUpgrades;
};
