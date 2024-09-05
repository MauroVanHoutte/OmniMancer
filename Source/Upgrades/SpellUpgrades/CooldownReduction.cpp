// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrades/SpellUpgrades/CooldownReduction.h"
#include "Upgrades/SpellUpgradesComponent.h"
#include "SpellCasting/ElementManipulationComponent.h"

void UCooldownReduction::OnUpgradeRegistered(USpellUpgradesComponent* Component, TSubclassOf<ABaseSpell> Spell)
{
	ElementManipulationComponent = Component->GetOwner()->GetComponentByClass<UElementManipulationComponent>();
	if (IsValid(ElementManipulationComponent))
	{
		ElementManipulationComponent->AddCooldownMultiplier(Spell, CooldownMultiplier);
	}
}

void UCooldownReduction::OnUpgradeRemoved(USpellUpgradesComponent* Component, TSubclassOf<ABaseSpell> Spell)
{
	if (IsValid(ElementManipulationComponent))
	{
		ElementManipulationComponent->AddCooldownMultiplier(Spell, FMath::Pow(1 / CooldownMultiplier, CurrentLevel));
	}
}

FFormatNamedArguments UCooldownReduction::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add("CooldownMultiplier", 100 - (CooldownMultiplier * 100));
	return Args;
}
