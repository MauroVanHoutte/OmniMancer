// Fill out your copyright notice in the Description page of Project Settings.


#include "FireRatePowerUpEffect.h"

void UFireRatePowerUpEffect::Apply(AWizardCharacter* target)
{
	target->m_BasicAttackCooldown *= (1 - Value/100.f);
}

void UFireRatePowerUpEffect::Remove(AWizardCharacter* target)
{
	target->m_BasicAttackCooldown /= (1 - Value / 100.f);
}
