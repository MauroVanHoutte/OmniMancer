// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedPowerUpEffect.h"
#include <GameFramework/CharacterMovementComponent.h>

void USpeedPowerUpEffect::Apply(AWizardCharacter* target)
{
	Cast<UCharacterMovementComponent>(target->GetMovementComponent())->MaxWalkSpeed *= (1 + Value / 100.f);
}

void USpeedPowerUpEffect::Remove(AWizardCharacter* target)
{
	Cast<UCharacterMovementComponent>(target->GetMovementComponent())->MaxWalkSpeed /= (1 + Value / 100.f);
}
