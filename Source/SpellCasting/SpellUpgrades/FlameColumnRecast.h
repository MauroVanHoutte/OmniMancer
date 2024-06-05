// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellCasting/BaseSpelUpgrade.h"
#include "FlameColumnRecast.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNREALPROJECT_API UFlameColumnRecast : public UBaseSpellUpgrade
{
	GENERATED_BODY()
public:
	virtual void ApplyToSpell(class ABaseSpell* Spell) override;

private:
	UFUNCTION()
	void OnFlameColumnDestroyed(AActor* DestroyedActor);
};
