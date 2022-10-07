// Fill out your copyright notice in the Description page of Project Settings.


#include "HealEffect.h"

void UHealEffect::Apply(AWizardCharacter* target)
{
	Duration = 0.f;
	target->Heal(HealAmount);
}

void UHealEffect::Remove(AWizardCharacter* target)
{
}
