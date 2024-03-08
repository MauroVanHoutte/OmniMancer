// Fill out your copyright notice in the Description page of Project Settings.


#include "SpreadPowerUpEffect.h"

void USpreadPowerUpEffect::Apply(AWizardCharacter* target)
{
	int spreadBefore{target->GetSpread()};
	spreadBefore += Value;
	target->SetSpread(spreadBefore);
}

void USpreadPowerUpEffect::Remove(AWizardCharacter* target)
{
	int spreadBefore{ target->GetSpread() };
	spreadBefore -= Value;
	target->SetSpread(spreadBefore);
}
