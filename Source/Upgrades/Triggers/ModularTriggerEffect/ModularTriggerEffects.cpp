// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularTriggerEffects.h"
#include <Engine/DamageEvents.h>
#include "Health/AffiliationComponent.h"
#include "Health/HealthManager.h"
#include "SpellCasting/Spells/BaseProjectile.h"
#include "SpellCasting/ElementManipulationComponent.h"
#include "StatusEffects/StatusEffectHandlingComponent.h"
#include "StatusEffects/StatusEffect.h"

void UStatusEffectTriggerEffect::ExecuteEffect(ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<AActor*>& targetActors, float Damage, class APawn* instigator)
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

void URadialProjectilesTriggerEffect::ExecuteEffect(ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	UWorld* World = instigator->GetWorld();
	if (ensure(World))
	{
		TArray<FVector> Locations = targetLocations;
		for (const AActor* Actor : targetActors)
		{
			Locations.Add(Actor->GetActorLocation());
		}

		UElementManipulationComponent* SpellCasting = instigator->GetComponentByClass<UElementManipulationComponent>();
		float AngleIncrement = UE_TWO_PI / ProjectileCount;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = instigator;
		SpawnParams.Owner = instigator;

		for (const FVector& Location : Locations)
		{
			float FiringAngle = 0;
			for (int ProjectileIdx = 0; ProjectileIdx < ProjectileCount; ProjectileIdx++)
			{
				ABaseProjectile* NewProjectile = Cast<ABaseProjectile>(SpellCasting->TriggeredCast(ProjectileType, FVector(0, 0, 0), Location, true));
				NewProjectile->FireInDirection(FVector(FMath::Sin(FiringAngle), FMath::Cos(FiringAngle), 0));

				FiringAngle += AngleIncrement;
			}
		}
	}
}

void UHealTargets::ExecuteEffect(ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
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

void UDamagePerHitActorOnSpellEndTriggerEffect::ExecuteEffect(ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	TriggeringSpell->OnSpellDestroyedDelegate.AddDynamic(this, &UDamagePerHitActorOnSpellEndTriggerEffect::OnSpellDestroyed);
}

void UDamagePerHitActorOnSpellEndTriggerEffect::OnSpellDestroyed(ABaseSpell* Spell)
{
	TArray<AActor*>& HitActors = Spell->GetHitActorsRef();
	float TotalDamage = HitActors.Num() * DamagePerTarget;
	FDamageEvent DamageEvent;
	for (AActor* Actor : HitActors)
	{
		Actor->TakeDamage(TotalDamage, DamageEvent, Spell->GetInstigatorController(), Spell->GetInstigator());
	}
}

void UCastSpellTriggerEffect::ExecuteEffect(ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	UElementManipulationComponent* SpellCasting = instigator->GetComponentByClass<UElementManipulationComponent>();
	if (IsValid(SpellCasting))
	{
		TArray<FVector> Locations = targetLocations;
		for (const AActor* Actor : targetActors)
		{
			Locations.Add(Actor->GetActorLocation());
		}

		for (const FVector& Location : Locations)
		{
			SpellCasting->TriggeredCast(SpellToCast, Location, bSpawnSpellAtInstigator ? instigator->GetActorLocation() : TriggeringSpell->GetActorLocation(), bSendSpellEvents);
		}
	}
}

void UCastOnSpellDestroyedTriggerEffect::ExecuteEffect(ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	if (!IsValid(Instigator))
	{
		Instigator = instigator;
	}

	if (IsValid(TriggeringSpell))
	{
		TriggeringSpell->OnSpellDestroyedDelegate.AddDynamic(this, &UCastOnSpellDestroyedTriggerEffect::CastSpell);
		TArray<FVector> Locations = targetLocations;
		for (const AActor* Actor : targetActors)
		{
			Locations.Add(Actor->GetActorLocation());
		}
		TargetLocations.Add(TriggeringSpell, Locations);
	}
}

void UCastOnSpellDestroyedTriggerEffect::CastSpell(ABaseSpell* Spell)
{
	UElementManipulationComponent* SpellCasting = Instigator->GetComponentByClass<UElementManipulationComponent>();
	if (TargetLocations.Contains(Spell) && IsValid(SpellCasting))
	{
		TArray<FVector>& Locations = TargetLocations[Spell];
		for (const FVector& Location : Locations)
		{
			SpellCasting->TriggeredCast(SpellToCast, Location, bSpawnSpellAtInstigator ? Instigator->GetActorLocation() : Spell->GetActorLocation(), bSendSpellEvents);
		}
	}
}

void URecastDamageSizeMulTriggerEffect::ExecuteEffect(ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	if (!IsValid(Instigator))
	{
		Instigator = instigator;
	}

	if (IsValid(TriggeringSpell))
	{
		TriggeringSpell->OnSpellDestroyedDelegate.AddDynamic(this, &URecastDamageSizeMulTriggerEffect::CastSpell);
	}
}

void URecastDamageSizeMulTriggerEffect::CastSpell(ABaseSpell* Spell)
{
	int CurrentRecastCount = RecastTracker.Contains(Spell) ? RecastTracker[Spell] : 0;
	RecastTracker.Remove(Spell);
	if (CurrentRecastCount < RecastCount)
	{
		UElementManipulationComponent* SpellCasting = Instigator->GetComponentByClass<UElementManipulationComponent>();
		if (IsValid(SpellCasting))
		{
			ABaseSpell* NewSpell = SpellCasting->TriggeredCast(Spell->GetClass(), Spell->GetActorLocation(), Spell->GetActorLocation(), bSendSpellEvents);
			NewSpell->SetScale(Spell->GetScale() * SizeMultiplier);
			NewSpell->SetBaseDamage(Spell->GetBaseDamage() * DamageMultiplier);
			CurrentRecastCount++;
			RecastTracker.Add(NewSpell, CurrentRecastCount);

			if (!bSendSpellEvents)
				NewSpell->OnSpellDestroyedDelegate.AddDynamic(this, &URecastDamageSizeMulTriggerEffect::CastSpell);
		}
	}
}

void UAddBouncesTriggerEffect::ExecuteEffect(ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	ABaseProjectile* Projectile = Cast<ABaseProjectile>(TriggeringSpell);
	if (IsValid(Projectile))
	{
		Projectile->SetBounces(Projectile->GetBounces() + ExtraBounces);
	}
}

void USummonTriggerEffect::ExecuteEffect(ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	for (const FVector& Location : targetLocations)
	{
		FActorSpawnParameters Params{};
		Params.Owner = instigator;
		Params.Instigator = instigator;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(*SummonedClass, Location, FRotator(0, 0, 0), Params);
		UAffiliationComponent* Affiliation = SpawnedActor->GetComponentByClass<UAffiliationComponent>();
		UAffiliationComponent* InstigatorAffiliation = IsValid(instigator) ? instigator->GetComponentByClass<UAffiliationComponent>() : nullptr;
		if (IsValid(Affiliation) && IsValid(InstigatorAffiliation))
		{
			Affiliation->SetAffiliation(InstigatorAffiliation->GetAffiliation());
		}

		if (SummonLimit > 0)
		{
			SummonQueue.Enqueue(SpawnedActor);
			if (QueueSize == SummonLimit)
			{
				AActor* OldSummon;
				SummonQueue.Dequeue(OldSummon);
				//Outer would sometimes be an unloaded level streaming resulting in a crash when calling destroy
				if (IsValid(OldSummon) && IsValid(OldSummon->GetOuter()))
				{
					OldSummon->Destroy();
				}
			}
			else
			{
				QueueSize++;
			}
		}
	}
}
