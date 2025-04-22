// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrades/ProxyTriggerHandlingComponent.h"

void UProxyTriggerHandlingComponent::BindProxiedComponent(UTriggerHandlingComponent* TargetComponent)
{
	ProxiedComponent = TargetComponent;
}

void UProxyTriggerHandlingComponent::TriggerDamageDealt(AActor* DamageCause, AActor* DamagedActor, float Damage)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerDamageDealt(DamageCause, DamagedActor, Damage);
	}
}

void UProxyTriggerHandlingComponent::TriggerEnemyKilled(AActor* DamageCause, AActor* KilledEnemy, float Damage)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerEnemyKilled(DamageCause, KilledEnemy, Damage);
	}
}

void UProxyTriggerHandlingComponent::TriggerSpellCasted(UElementManipulationComponent* CastingComponent, UBasePlayerCast* CastObject, ABaseSpell* Spell)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerSpellCasted(CastingComponent, CastObject, Spell);
	}
}

void UProxyTriggerHandlingComponent::TriggerBasicAttackCasted(UElementManipulationComponent* CastingComponent, UBasePlayerCast* CastObject, ABaseSpell* Spell)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerBasicAttackCasted(CastingComponent, CastObject, Spell);
	}
}

void UProxyTriggerHandlingComponent::TriggerSpellHit(UElementManipulationComponent* CastingComponent, UBasePlayerCast* CastObject, ABaseSpell* Spell, AActor* HitActor)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerSpellHit(CastingComponent, CastObject, Spell, HitActor);
	}
}

void UProxyTriggerHandlingComponent::TriggerBasicAttackHit(UElementManipulationComponent* CastingComponent, UBasePlayerCast* CastObject, ABaseSpell* Spell, AActor* HitActor)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerBasicAttackHit(CastingComponent, CastObject, Spell, HitActor);
	}
}

void UProxyTriggerHandlingComponent::TriggerDamageTaken(UBaseHealthComponent* DamagedComponent, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerDamageTaken(DamagedComponent, Damage, DamageType, InstigatedBy, DamageCauser);
	}
}
