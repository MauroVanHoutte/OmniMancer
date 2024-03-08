// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedPowerUpEffect.h"
#include <GameFramework/CharacterMovementComponent.h>

void USpeedPowerUpEffect::Apply(AWizardCharacter* target)
{
	float moveSpeedBefore{ target->GetSpeed() };
	moveSpeedBefore *= (1 + Value);
	target->SetSpeed(moveSpeedBefore);
}

void USpeedPowerUpEffect::Remove(AWizardCharacter* target)
{
	float moveSpeedBefore{ target->GetSpeed() };
	moveSpeedBefore /= (1 + Value);
	target->SetSpeed(moveSpeedBefore);
}
