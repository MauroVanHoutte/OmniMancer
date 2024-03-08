// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularExtraConditions.h"
#include "SpellCasting/Spells/BaseSpell.h"

bool UCompoundModularCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	bool CheckOne = ConditionOne->CheckCondition(triggerOwner, spell, target);
	bool CheckTwo = ConditionTwo->CheckCondition(triggerOwner, spell, target);

	switch (Operation)
	{
	case CompoundConditionOperation::AND:
		return CheckOne && CheckTwo;
		break;
	case CompoundConditionOperation::OR:
		return CheckOne || CheckTwo;
		break;
	default:
		break;
	}
	return false;
}

bool USpellCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	return spell->IsA(RequiredSpell);
}
