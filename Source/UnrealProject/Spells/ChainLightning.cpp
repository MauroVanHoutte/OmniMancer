// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainLightning.h"
#include <Kismet/GameplayStatics.h>
#include "../Enemies/BaseCharacter.h"
#include "../WizardCharacter.h"

AChainLightning::AChainLightning()
	: ABaseProjectile()
{
}

void AChainLightning::BeginPlay()
{
	Super::BeginPlay();
	Damage = InitialDamage;
	if (Stuns)
	{
		StatusEffects.Add(FStatusEffect(Type::Stun, -1.f, -1.f, StunDuration, this));
	}

	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->InitialSpeed = MaxSpeed;
}

