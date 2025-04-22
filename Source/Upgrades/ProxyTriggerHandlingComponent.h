// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Upgrades/TriggerHandlingComponent.h"
#include "ProxyTriggerHandlingComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UProxyTriggerHandlingComponent : public UTriggerHandlingComponent
{

	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void BindProxiedComponent(UTriggerHandlingComponent* TargetComponent);
	void TriggerDamageDealt(AActor* DamageCause, AActor* DamagedActor, float Damage) override;
	void TriggerEnemyKilled(AActor* DamageCause, AActor* KilledEnemy, float Damage) override;
	void TriggerSpellCasted(class UElementManipulationComponent* CastingComponent, class UBasePlayerCast* CastObject, class ABaseSpell* Spell) override;
	void TriggerBasicAttackCasted(class UElementManipulationComponent* CastingComponent, class UBasePlayerCast* CastObject, class ABaseSpell* Spell) override;
	void TriggerSpellHit(class UElementManipulationComponent* CastingComponent, class UBasePlayerCast* CastObject, class ABaseSpell* Spell, AActor* HitActor) override;
	void TriggerBasicAttackHit(class UElementManipulationComponent* CastingComponent, class UBasePlayerCast* CastObject, class ABaseSpell* Spell, AActor* HitActor) override;
	void TriggerDamageTaken(class UBaseHealthComponent* DamagedComponent, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;

private:
	UPROPERTY(Transient)
	UTriggerHandlingComponent* ProxiedComponent;
};
