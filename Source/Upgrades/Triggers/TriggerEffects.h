// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Health/BaseHealthComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include "ModularExtraConditions/ModularExtraConditions.h"
#include "ModularTriggerEffect/ModularTriggerEffects.h"
#include "ModularTriggerTargetting/ModularTriggerTargetting.h"
#include "SpellCasting/Spells/BaseSpell.h"
#include "SpellCasting/Spells/BaseProjectile.h"
#include "SpellCasting/Spells/Blizzard.h"
#include "SpellCasting/Spells/Vortex.h"
#include "StatusEffects/StatusEffect.h"
#include "StatusEffects/StatusEffectHandlingComponent.h"
#include <Engine/DamageEvents.h>
#include "TriggerEffects.generated.h"

UCLASS(Abstract, EditInlineNew)
class UBaseTriggerEffect : public UObject
{
	GENERATED_BODY()
public:
	virtual void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) {};
	UPROPERTY(BlueprintReadOnly)
	class APawn* Instigator;
	virtual FFormatNamedArguments GetDescriptionArguments() { return FFormatNamedArguments(); };
};

UCLASS(Abstract, Blueprintable, EditInlineNew)
class UBPBaseTriggerEffect : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		BP_OnTrigger(triggerOwner, spell, target, Damage);
	};

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTrigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage);
};

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class UModularTriggerEffect : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		if (!IsValid(EffectModule) ||
			(IsValid(ConditionsModule) &&
			(bInvertCondition ? ConditionsModule->CheckCondition(triggerOwner, spell, target) : !ConditionsModule->CheckCondition(triggerOwner, spell, target))))
		{
			return;
		}

		TArray<FVector> targetLocations;
		TArray<AActor*> targetActors;
		if (IsValid(TargettingModule))
		{
			TargettingModule->GatherTargets(triggerOwner, spell, target, targetLocations, targetActors);
		}

		if (IsValid(ConditionsModule))
		{
			ConditionsModule->OnExecution(targetLocations, targetActors, Damage, Instigator);
		}

		EffectModule->ExecuteEffect(spell, targetLocations, targetActors, Damage, Instigator);
	}

	virtual FFormatNamedArguments GetDescriptionArguments() 
	{ 
		FFormatNamedArguments Arguments;
		if (IsValid(ConditionsModule))
			Arguments.Append(ConditionsModule->GetDescriptionArguments());
		if (IsValid(TargettingModule))
			Arguments.Append(TargettingModule->GetDescriptionArguments());
		if (IsValid(EffectModule))
			Arguments.Append(EffectModule->GetDescriptionArguments());
		
		return Arguments; 
	};

private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UModularExtraConditionsBase* ConditionsModule;
	UPROPERTY(EditDefaultsOnly)
	bool bInvertCondition = false;
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UModularTriggerTargettingBase* TargettingModule;
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UModularTriggerEffectBase* EffectModule;
};

UCLASS(BlueprintType, EditInlineNew)
class UStatusEffectTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		UStatusEffectHandlingComponent* StatusEffectHandler = target->GetComponentByClass<UStatusEffectHandlingComponent>();

		if (StatusEffectHandler)
		{
			StatusEffectHandler->ApplyStatusEffect(StatusEffect);
		}
	};

private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	UBaseStatusEffect* StatusEffect;
};

//Slow enemies in radius around caster
UCLASS(BlueprintType, EditInlineNew)
class UAoeStatusEffectTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		TArray<AActor*> ignore{ triggerOwner };
		TArray<AActor*> outActors;
		//UKismetSystemLibrary::SphereOverlapActors(triggerOwner->GetWorld(), triggerOwner->GetActorLocation(), Radius, TArray<TEnumAsByte<EObjectTypeQuery>>(), ABaseCharacter::StaticClass(), ignore, outActors);
		for (AActor* actor : outActors)
		{
			//auto enemy = Cast<ABaseCharacter>(actor);
			//if (IsValid(enemy))
				//enemy->AddStatusEffect(FStatusEffect{ Type::Slow, -1, Slow , Duration, triggerOwner });
		}
	};

private:
	UPROPERTY(EditDefaultsOnly)
	float Radius = 600;
	UPROPERTY(EditDefaultsOnly)
	float Slow = 0.2;
	UPROPERTY(EditDefaultsOnly)
	float Duration = 3;
};

//Give the caster a speed boost
UCLASS(BlueprintType, EditInlineNew)
class USpeedBuffTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		//triggerOwner->AddStatusEffect(FStatusEffect{ Type::Slow, -1, -Speed, Duration, triggerOwner });
	};

private:
	UPROPERTY(EditDefaultsOnly)
	float Speed = 0.2;
	UPROPERTY(EditDefaultsOnly)
	float Duration = 3;
};

//Chance to lower Cooldowns
UCLASS(BlueprintType, EditInlineNew)
class UBaseAttackLowerCooldownTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		//only execute if base projectile
		auto baseProjectile = Cast<ABaseProjectile>(spell);
		if (baseProjectile == nullptr)
			return;

		float rand = FMath::FRandRange(0.f, 1.f);
		//if (rand < Chance)
			//triggerOwner->LowerCooldowns(Amount);
	};

private:
	UPROPERTY(EditDefaultsOnly)
	float Chance = 0.3f;
	UPROPERTY(EditDefaultsOnly)
	float Amount = 1.f;
};

//Temp Fire Rate
UCLASS(BlueprintType, EditInlineNew)
class UBaseAttackTempFireRateTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		//only execute if base projectile
		auto baseProjectile = Cast<ABaseProjectile>(spell);
		if (baseProjectile == nullptr)
			return;

		//float currentCooldown = triggerOwner->GetBasicAttackCooldown();
		//triggerOwner->SetBasicAttackCooldown(currentCooldown / FireRateMultiplier);

		//undo buff after duration
		FTimerHandle handle;
		triggerOwner->GetWorld()->GetTimerManager().SetTimer(handle, [triggerOwner, this]()
			{
				//float currentCooldown = triggerOwner->GetBasicAttackCooldown();
				//triggerOwner->SetBasicAttackCooldown(currentCooldown * FireRateMultiplier);
			}, Duration, false);
	};

private:
	UPROPERTY(EditDefaultsOnly)
	float FireRateMultiplier = 1.1f;
	UPROPERTY(EditDefaultsOnly)
	float Duration = 3.f;

};

//Temp Spell Damage on Kill
UCLASS(BlueprintType, EditInlineNew)
class USpellDamageOnKillTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		//only execute if target died
		if (target == nullptr || !target->IsActorBeingDestroyed())
			return;

		//float currentSpellDamageMultiplier = triggerOwner->GetSpellDamageMultiplier();
		//triggerOwner->SetSpellDamageMultiplier(currentSpellDamageMultiplier * SpellDamageBoost);

		//undo buff after duration
		FTimerHandle handle;
		triggerOwner->GetWorld()->GetTimerManager().SetTimer(handle, [triggerOwner, this]()
			{
				//float currentSpellDamageMultiplier = triggerOwner->GetSpellDamageMultiplier();
				//triggerOwner->SetSpellDamageMultiplier(currentSpellDamageMultiplier / SpellDamageBoost);
			}, Duration, false);
	};

private:
	UPROPERTY(EditDefaultsOnly)
	float SpellDamageBoost = 1.15f;
	UPROPERTY(EditDefaultsOnly)
	float Duration = 7.f;
};

//Blizzard Trigger Effect
UCLASS(BlueprintType, EditInlineNew)
class UBlizzardOnSlowTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		//slows from blizzard itself dont count
		ABlizzard* blizzard = Cast<ABlizzard>(spell);
		if (spell == nullptr || blizzard != nullptr || target == nullptr)
			return;

		if (!bInitialized)
		{
			Blizzard = GetWorld()->SpawnActor<ABlizzard>(*BlizzardClass);
			Blizzard->SetParent(Cast<APawn>(triggerOwner));
			bInitialized = true;
		}

		auto slowEffect = spell->GetStatusEffectsRef().FindByPredicate([](const UBaseStatusEffect* effect) {return effect->IsA<UMovementSpeedStatusEffect>(); });
		if (slowEffect != nullptr)
		{
			FTimerHandle* ActiveActorTimer = ActiveSlows.Find(target);

			if (!ActiveActorTimer)
			{
				CurrentSlowsActive++;
				UE_LOG(LogTemp, Warning, TEXT("slow added"));
				if (CurrentSlowsActive >= SlowsNeeded)
				{
					GetWorld()->GetTimerManager().ClearTimer(DeactivationTimer);
					Blizzard->Activate();
					UE_LOG(LogTemp, Warning, TEXT("Blizzard Activated"));
				}
			}
			else
			{
				triggerOwner->GetWorld()->GetTimerManager().ClearTimer(*ActiveActorTimer);
			}

			FTimerHandle handle;
			triggerOwner->GetWorld()->GetTimerManager().SetTimer(handle, [triggerOwner, target, this](){
				CurrentSlowsActive--;
				UE_LOG(LogTemp, Warning, TEXT("slow removed"));

				ActiveSlows.Remove(target);

				if (CurrentSlowsActive == SlowsNeeded-1)
				{
					GetWorld()->GetTimerManager().SetTimer(DeactivationTimer, Blizzard, &ABlizzard::Deactivate, LingerDuration);
					UE_LOG(LogTemp, Warning, TEXT("Blizzard deactivates soon"));
				}}, (*slowEffect)->TotalDuration, false);
			
			ActiveSlows.Add(target, handle);
		}
	};

private:
	int CurrentSlowsActive = 0;
	UPROPERTY(EditDefaultsOnly)
	int SlowsNeeded = 4;
	UPROPERTY(EditDefaultsOnly)
	float LingerDuration = 3.f;
	FTimerHandle DeactivationTimer;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABlizzard> BlizzardClass;
	ABlizzard* Blizzard;
	bool bInitialized = false;
	UPROPERTY(Transient)
	TMap<AActor*, FTimerHandle> ActiveSlows;
};

//Damage Reduction Trigger
UCLASS(BlueprintType, EditInlineNew)
class UDamageReductionTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		//triggerOwner->SetDamageTakenMultiplier(triggerOwner->GetDamageTakenMultiplier() * Amount);
			
		FTimerHandle handle;
	//	triggerOwner->GetWorld()->GetTimerManager().SetTimer(handle, [triggerOwner, this]() {
	//		triggerOwner->SetDamageTakenMultiplier(triggerOwner->GetDamageTakenMultiplier() / Amount);
	//		}, Duration, false);
	};

private:
	UPROPERTY(EditDefaultsOnly)
	float Duration = 3.f;
	UPROPERTY(EditDefaultsOnly)
	float Amount = 0.85f;
};

//Vortex chance on Kill
UCLASS(BlueprintType, EditInlineNew)
class UVortexOnKillTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		//only execute if target died
		if (target == nullptr || !target->IsActorBeingDestroyed())
			return;

		if (FMath::FRand() > SpawnChance)
			return;

		AVortex* vortex = GetWorld()->SpawnActor<AVortex>(*VortexClass);
		//vortex->InitSpell(target->GetActorLocation(), triggerOwner);
	};

private:
	UPROPERTY(EditDefaultsOnly)
	float SpawnChance = 0.3f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AVortex> VortexClass;
};

UCLASS(BlueprintType, EditInlineNew)
class UHealHealthCompTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		if (IsValid(HealthComp))
		{
			HealthComp->Heal(HealAmount);
		}
	};

	void SetHealthComponent(UBaseHealthComponent* NewHealthComponent)
	{
		HealthComp = NewHealthComponent;
	}
	
private:
	UPROPERTY(Transient)
	UBaseHealthComponent* HealthComp;
	UPROPERTY(EditDefaultsOnly)
	float HealAmount = 10.f;
};

UCLASS(BlueprintType, EditInlineNew)
class UDamagePerHitActorOnSpellEndTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void Trigger(AActor* triggerOwner, ABaseSpell* spell, AActor* target, float Damage) override
	{
		spell->OnSpellDestroyedDelegate.AddDynamic(this, &UDamagePerHitActorOnSpellEndTrigger::OnSpellEnd);
	}

private:
	UFUNCTION()
	void OnSpellEnd(class ABaseSpell* Spell)
	{
		TArray<AActor*>& HitActors = Spell->GetHitActorsRef();
		float TotalDamage = DamagePerTarget * HitActors.Num();
		for (AActor* Actor : HitActors)
		{
			Actor->TakeDamage(TotalDamage, FDamageEvent{}, Instigator->GetInstigatorController(), Instigator);
		}
	}

	UPROPERTY(EditDefaultsOnly)
	float DamagePerTarget = 1.f;
};