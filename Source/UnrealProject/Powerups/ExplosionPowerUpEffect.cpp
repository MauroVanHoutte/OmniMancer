// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionPowerUpEffect.h"

void UExplosionPowerUpEffect::Apply(AWizardCharacter* target)
{
	target->m_ExplosiveBaseAttack = true;
	target->m_ExplosionDamage += Damage;
	target->m_ExplosionRadius += Radius;
}

void UExplosionPowerUpEffect::Remove(AWizardCharacter* target)
{
	target->m_ExplosionDamage -= Damage;
	target->m_ExplosionRadius -= Radius;
	if (FMath::IsNearlyEqual(target->m_ExplosionDamage, 0.f))
	{
		target->m_ExplosiveBaseAttack = false;
	}
}
