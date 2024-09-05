// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrades/SpellUpgrades/AddStatusEffect.h"
#include "SpellCasting/Spells/BaseSpell.h"

void UAddStatusEffect::ApplyToSpell(ABaseSpell* Spell)
{
	Spell->GetStatusEffectsRef().Add(StatusEffect);
}
