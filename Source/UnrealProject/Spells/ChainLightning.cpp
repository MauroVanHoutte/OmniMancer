// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainLightning.h"
#include <Kismet/GameplayStatics.h>
#include "../Enemies/BaseCharacter.h"
#include "../WizardCharacter.h"

AChainLightning::AChainLightning()
	: ABaseProjectile()
{
}

void AChainLightning::InitSpell(const FVector& casterLocation, const FVector& targetLocation, const FVector& projectileDirection, AActor* owner, APawn* instigator, int fireLevel, int frostLevel, int windLevel)
{
	SetOwner(owner);
	SetInstigator(instigator);
	SetActorLocation(casterLocation);
	FireInDirection(projectileDirection);
}

void AChainLightning::BeginPlay()
{
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->InitialSpeed = MaxSpeed;
	Super::BeginPlay();
	Damage = InitialDamage;
	if (Stuns)
	{
		StatusEffects.Add(FStatusEffect(Type::Stun, -1.f, -1.f, StunDuration, this));
	}
}

