// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularTriggerEffects.h"
#include "Health/HealthManager.h"
#include "StatusEffects/StatusEffectHandlingComponent.h"
#include "StatusEffects/StatusEffect.h"
#include "SpellCasting/Spells/BaseProjectile.h"
#include "SpellCasting/ElementManipulationComponent.h"

void UStatusEffectTriggerEffect::ExecuteEffect(const TArray<FVector>& targetLocations, const TArray<AActor*>& targetActors, float Damage, class APawn* instigator)
{
	StatusEffect->Instigator = instigator;
	for (AActor* Actor : targetActors)
	{
		UStatusEffectHandlingComponent* StatusEffectHandlingComp = Actor->GetComponentByClass<UStatusEffectHandlingComponent>();
		if (IsValid(StatusEffectHandlingComp))
		{
			StatusEffectHandlingComp->ApplyStatusEffect(StatusEffect);
		}
	}
}

void URadialProjectilesTriggerEffect::ExecuteEffect(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	UWorld* World = instigator->GetWorld();
	if (ensure(World))
	{
		UElementManipulationComponent* SpellCasting = instigator->GetComponentByClass<UElementManipulationComponent>();
		float FiringAngle = 0;
		float AngleIncrement = UE_TWO_PI / ProjectileCount;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = instigator;
		SpawnParams.Owner = instigator;

		for (int ProjectileIdx = 0; ProjectileIdx < ProjectileCount; ProjectileIdx++)
		{
			ABaseProjectile* NewProjectile = Cast<ABaseProjectile>(SpellCasting->TriggeredCast(ProjectileType, true));
			NewProjectile->FireInDirection(FVector(FMath::Sin(FiringAngle), FMath::Cos(FiringAngle), 0));

			FiringAngle += AngleIncrement;
		}
	}
}

void UHealTargets::ExecuteEffect(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	for (const AActor* Actor : targetActors)
	{
		UHealthManager* HealthManager = Actor->GetComponentByClass<UHealthManager>();
		if (IsValid(HealthManager))
		{
			HealthManager->Heal(HealAmount);
		}
	}
}
