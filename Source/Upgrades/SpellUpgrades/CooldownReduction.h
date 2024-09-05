// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Upgrades/SpellUpgrades/BaseSpelUpgrade.h"
#include "CooldownReduction.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNREALPROJECT_API UCooldownReduction : public UBaseSpellUpgradeEffect
{
	GENERATED_BODY()
	
public:
	virtual void OnUpgradeRegistered(class USpellUpgradesComponent* Component, TSubclassOf<ABaseSpell> Spell) override;
	virtual void OnUpgradeRemoved(class USpellUpgradesComponent* Component, TSubclassOf<ABaseSpell> Spell) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float CooldownMultiplier = 0.75f;
	UPROPERTY(Transient)
	class UElementManipulationComponent* ElementManipulationComponent;
};
