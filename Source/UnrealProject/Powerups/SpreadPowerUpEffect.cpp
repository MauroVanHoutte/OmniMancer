// Fill out your copyright notice in the Description page of Project Settings.


#include "SpreadPowerUpEffect.h"

void USpreadPowerUpEffect::Apply(AWizardCharacter* target)
{
	target->m_Spread += Value;
}

void USpreadPowerUpEffect::Remove(AWizardCharacter* target)
{
	target->m_Spread -= Value;
}
