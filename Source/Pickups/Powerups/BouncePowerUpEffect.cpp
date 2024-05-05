// Fill out your copyright notice in the Description page of Project Settings.


#include "BouncePowerUpEffect.h"

void UBouncePowerUpEffect::Apply(AWizardCharacter* target)
{
	int before = target->GetBounces();
	target->SetBounces(before + Bounces);
}

void UBouncePowerUpEffect::Remove(AWizardCharacter* target)
{
	int before = target->GetBounces();
	target->SetBounces(before - Bounces);
}
