// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowPoweUpEffect.h"

void USlowPoweUpEffect::Apply(AWizardCharacter* target)
{
	auto& effects = target->GetBaseAttackEffectsRef();
	FStatusEffect effect = FStatusEffect(Type::Slow, -1, Amount, Duration, target);
	effects.Add(effect);
}

void USlowPoweUpEffect::Remove(AWizardCharacter* target)
{
	auto& effects = target->GetBaseAttackEffectsRef();

	for (int i = 0; i < effects.Num(); i++)
	{
		if (effects[i].EffectType == Type::Slow && effects[i].Value == Amount && effects[i].Cause == this)
		{
			effects.RemoveAt(i);
		}
	}
}