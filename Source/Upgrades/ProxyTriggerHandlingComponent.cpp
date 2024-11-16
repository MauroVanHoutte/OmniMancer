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

void UProxyTriggerHandlingComponent::TriggerSpellCasted(AActor* Caster, ABaseSpell* Spell)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerSpellCasted(Caster, Spell);
	}
}

void UProxyTriggerHandlingComponent::TriggerBasicAttackCasted(AActor* Caster, ABaseSpell* Spell)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerBasicAttackCasted(Caster, Spell);
	}
}

void UProxyTriggerHandlingComponent::TriggerSpellHit(ABaseSpell* Spell, AActor* HitActor)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerSpellHit(Spell, HitActor);
	}
}

void UProxyTriggerHandlingComponent::TriggerBasicAttackHit(ABaseSpell* Spell, AActor* HitActor)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerBasicAttackHit(Spell, HitActor);
	}
}

void UProxyTriggerHandlingComponent::TriggerDamageTaken(UBaseHealthComponent* DamagedComponent, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(ProxiedComponent))
	{
		ProxiedComponent->TriggerDamageTaken(DamagedComponent, Damage, DamageType, InstigatedBy, DamageCauser);
	}
}
