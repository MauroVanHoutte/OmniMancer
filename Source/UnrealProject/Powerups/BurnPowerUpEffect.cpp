// Fill out your copyright notice in the Description page of Project Settings.


#include "BurnPowerUpEffect.h"
#include "../StatusEffect.h"

void UBurnPowerUpEffect::Apply(AWizardCharacter* target)
{
	auto& effects = target->GetBaseAttackEffectsRef();
	FStatusEffect effect = FStatusEffect(Type::Damage, Interval, Damage, Duration, target);
	auto existingEffect = effects.FindByPredicate([this](const FStatusEffect& effect) {return effect.EffectType == Type::Damage && effect.Interval == this->Interval; });

	if (existingEffect == nullptr)
	{
		effects.Add(effect);
	}
	else
	{
		existingEffect->Value += Damage;
	}
}

void UBurnPowerUpEffect::Remove(AWizardCharacter* target)
{
	auto& effects = target->GetBaseAttackEffectsRef();

	for (int i = 0; i < effects.Num(); i++)
	{
		if (effects[i].EffectType == Type::Damage && effects[i].Interval == Interval)
		{
			effects[i].Value -= Damage;
			if (effects[i].Value < Damage)
				effects.RemoveAt(i);
		}
	}
}