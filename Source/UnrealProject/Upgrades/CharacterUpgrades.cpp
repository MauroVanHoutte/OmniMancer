#include "CharacterUpgrades.h"
#include "../WizardCharacter.h"
#include "Triggers/TriggerEffects.h"
#include "StatUpgrades/StatBoosts.h"
#include "RepeatingEffects/RepeatingEffect.h"

void UTriggerUpgrade::Apply(AWizardCharacter* character)
{
	if (!Applied)
	{
		character->AddTriggerEffect(TriggerEffect);
		Applied = true;
	}
}

void UTriggerUpgrade::Remove(AWizardCharacter* character)
{
	if (Applied)
	{
		character->RemoveTriggerEffect(TriggerEffect);
		Applied = false;
	}
}

void UStatUpgrade::Remove(AWizardCharacter* character)
{
	if (Applied)
	{
		StatBoost->Remove(character);
		Applied = false;
	}
}

void UStatUpgrade::Apply(AWizardCharacter* character)
{
	if (!Applied)
	{
		StatBoost->Apply(character);
		Applied = true;
	}
}

void UBaseAttackStatusEffectUpgrade::Apply(AWizardCharacter* character)
{
	if (!Applied)
	{
		character->AddBaseAttackStatusEffect(StatusEffect);
		Applied = true;
	}
}

void UBaseAttackStatusEffectUpgrade::Remove(AWizardCharacter* character)
{
	if (Applied)
	{
		character->RemoveBaseAttackStatusEffect(StatusEffect);
		Applied = false;
	}
}

void UReflectStatusEffectUpgrade::Apply(AWizardCharacter* character)
{
	if (!Applied)
	{
		character->AddReflectStatusEffect(StatusEffect);
		Applied = true;
	}
}

void UReflectStatusEffectUpgrade::Remove(AWizardCharacter* character)
{
	if (Applied)
	{
		character->RemoveReflectStatusEffect(StatusEffect);
		Applied = false;
	}
}

void URepeatingEffectUpgrade::Apply(AWizardCharacter* character)
{
	if (!Applied)
	{
		Effect->Apply(character);
		Applied = true;
	}
}

void URepeatingEffectUpgrade::Remove(AWizardCharacter* character)
{
	if (Applied)
	{
		Effect->Remove(character);
		Applied = false;
	}
}
