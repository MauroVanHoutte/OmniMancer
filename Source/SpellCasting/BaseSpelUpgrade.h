// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseSpelUpgrade.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, EditInlineNew)
class USpellUpgradeData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	class UBaseSpellUpgrade* Upgrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class ABaseSpell>> ApplicableSpells;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Priority = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Limit = 1;
};

UCLASS(Abstract, BlueprintType, EditInlineNew)
class UNREALPROJECT_API UBaseSpellUpgrade : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void ApplyToSpell(class ABaseSpell* Spell) {};
};
