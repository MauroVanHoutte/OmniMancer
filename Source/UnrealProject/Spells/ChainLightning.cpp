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
	m_Damage = m_InitialDamage;
	if (m_Stuns)
	{
		m_StatusEffects.Add(FStatusEffect(Type::Stun, -1.f, -1.f, m_StunDuration, this));
	}

	m_ProjectileMovement->MaxSpeed = m_MaxSpeed;
	m_ProjectileMovement->InitialSpeed = m_MaxSpeed;
}

