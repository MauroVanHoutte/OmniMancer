// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Upgrades/SpellUpgrades/BaseSpelUpgrade.h"
#include "AddStatusEffect.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNREALPROJECT_API UAddStatusEffect : public UBaseSpellUpgradeEffect
{
	GENERATED_BODY()
public:
	virtual void ApplyToSpell(class ABaseSpell* Spell) override;

private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBaseStatusEffect* StatusEffect;
};
