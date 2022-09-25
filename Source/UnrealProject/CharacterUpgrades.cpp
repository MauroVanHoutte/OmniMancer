#include "CharacterUpgrades.h"
#include "WizardCharacter.h"
#include "TriggerEffects.h"

void UTriggerUpgrade::Apply(AWizardCharacter* character)
{
	character->AddTriggerEffect(TriggerEffect);
}

void UTriggerUpgrade::Remove(AWizardCharacter* character)
{
	character->RemoveTriggerEffect(TriggerEffect, Tag);
}
