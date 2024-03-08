// Fill out your copyright notice in the Description page of Project Settings.


#include "FireRatePowerUpEffect.h"

void UFireRatePowerUpEffect::Apply(AWizardCharacter* target)
{
	float attackCooldownBefore{ target->GetBasicAttackCooldown() };
	attackCooldownBefore *= (1 - Value);
	target->SetBasicAttackCooldown(attackCooldownBefore);
}

void UFireRatePowerUpEffect::Remove(AWizardCharacter* target)
{
	float attackCooldownBefore{ target->GetBasicAttackCooldown() };
	attackCooldownBefore /= (1 - Value);
	target->SetBasicAttackCooldown(attackCooldownBefore);
}
