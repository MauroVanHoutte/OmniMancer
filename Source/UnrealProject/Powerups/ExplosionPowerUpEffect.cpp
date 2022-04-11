// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionPowerUpEffect.h"

void UExplosionPowerUpEffect::Apply(AWizardCharacter* target)
{
	float damageBefore{};
	float radiusBefore{};

	target->GetExplosionVariables(damageBefore, radiusBefore);
	target->SetExplosionVariables(damageBefore + Damage, radiusBefore + Radius, true);
}

void UExplosionPowerUpEffect::Remove(AWizardCharacter* target)
{
	float damageBefore{};
	float radiusBefore{};
	target->GetExplosionVariables(damageBefore, radiusBefore);
	damageBefore -= Damage;
	radiusBefore -= Radius;
	bool explode = true;
	if (FMath::IsNearlyEqual(damageBefore, 0.f))
	{
		explode = false;
	}

	target->SetExplosionVariables(damageBefore, radiusBefore, explode);
}
