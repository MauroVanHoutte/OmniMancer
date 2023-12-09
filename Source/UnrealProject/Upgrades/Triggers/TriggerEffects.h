// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../WizardCharacter.h"
#include "../../Spells/BaseSpell.h"
#include "../../Spells/BaseProjectile.h"
#include "../../Spells/Blizzard.h"
#include "../../Spells/Vortex.h"
#include <Kismet/KismetSystemLibrary.h>
#include "TriggerEffects.generated.h"

UCLASS(Abstract, EditInlineNew)
class UBaseTriggerEffect : public UObject
{
	GENERATED_BODY()
public:
	virtual void OnTrigger(AWizardCharacter* triggerOwner, ABaseSpell* spell, ABaseCharacter* target) {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TriggerCondition Condition{};
};

UCLASS(Abstract, Blueprintable, EditInlineNew)
class UBPBaseTriggerEffect : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void OnTrigger(AWizardCharacter* triggerOwner, ABaseSpell* spell, ABaseCharacter* target) override
	{
		BP_OnTrigger(triggerOwner, spell, target);
	};

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTrigger(AWizardCharacter* triggerOwner, ABaseSpell* spell, ABaseCharacter* target);
};

//Slow enemies in radius around caster
UCLASS(BlueprintType, EditInlineNew)
class UAoeSlowTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void OnTrigger(AWizardCharacter* triggerOwner, ABaseSpell* spell, ABaseCharacter* target) override
	{
		TArray<AActor*> ignore{ triggerOwner };
		TArray<AActor*> outActors;
		UKismetSystemLibrary::SphereOverlapActors(triggerOwner->GetWorld(), triggerOwner->GetActorLocation(), Radius, TArray<TEnumAsByte<EObjectTypeQuery>>(), ABaseCharacter::StaticClass(), ignore, outActors);
		for (AActor* actor : outActors)
		{
			auto enemy = Cast<ABaseCharacter>(actor);
			if (IsValid(enemy))
				enemy->AddStatusEffect(FStatusEffect{ Type::Slow, -1, Slow , Duration, triggerOwner });
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
	void OnTrigger(AWizardCharacter* triggerOwner, ABaseSpell* spell, ABaseCharacter* target) override
	{
		triggerOwner->AddStatusEffect(FStatusEffect{ Type::Slow, -1, -Speed, Duration, triggerOwner });
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
	void OnTrigger(AWizardCharacter* triggerOwner, ABaseSpell* spell, ABaseCharacter* target) override
	{
		//only execute if base projectile
		auto baseProjectile = Cast<ABaseProjectile>(spell);
		if (baseProjectile == nullptr)
			return;

		float rand = FMath::FRandRange(0.f, 1.f);
		if (rand < Chance)
			triggerOwner->LowerCooldowns(Amount);
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
	void OnTrigger(AWizardCharacter* triggerOwner, ABaseSpell* spell, ABaseCharacter* target) override
	{
		//only execute if base projectile
		auto baseProjectile = Cast<ABaseProjectile>(spell);
		if (baseProjectile == nullptr)
			return;

		float currentCooldown = triggerOwner->GetBasicAttackCooldown();
		triggerOwner->SetBasicAttackCooldown(currentCooldown / FireRateMultiplier);

		//undo buff after duration
		FTimerHandle handle;
		triggerOwner->GetWorld()->GetTimerManager().SetTimer(handle, [triggerOwner, this]()
			{
				float currentCooldown = triggerOwner->GetBasicAttackCooldown();
				triggerOwner->SetBasicAttackCooldown(currentCooldown * FireRateMultiplier);
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
	void OnTrigger(AWizardCharacter* triggerOwner, ABaseSpell* spell, ABaseCharacter* target) override
	{
		//only execute if target died
		if (target == nullptr || !target->IsActorBeingDestroyed())
			return;

		float currentSpellDamageMultiplier = triggerOwner->GetSpellDamageMultiplier();
		triggerOwner->SetSpellDamageMultiplier(currentSpellDamageMultiplier * SpellDamageBoost);

		//undo buff after duration
		FTimerHandle handle;
		triggerOwner->GetWorld()->GetTimerManager().SetTimer(handle, [triggerOwner, this]()
			{
				float currentSpellDamageMultiplier = triggerOwner->GetSpellDamageMultiplier();
				triggerOwner->SetSpellDamageMultiplier(currentSpellDamageMultiplier / SpellDamageBoost);
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
	void OnTrigger(AWizardCharacter* triggerOwner, ABaseSpell* spell, ABaseCharacter* target) override
	{
		//slows from blizzard itself dont count
		ABlizzard* blizzard = Cast<ABlizzard>(spell);
		if (spell == nullptr || blizzard != nullptr)
			return;

		if (!Initialized)
		{
			Blizzard = GetWorld()->SpawnActor<ABlizzard>(*BlizzardClass);
			Blizzard->SetWizard(triggerOwner);
			Initialized = true;
		}

		auto slowEffect = spell->GetStatusEffects().FindByPredicate([](const FStatusEffect& effect) {return effect.EffectType == Type::Slow; });
		if (slowEffect != nullptr)
		{
			CurrentSlowsActive++;
			UE_LOG(LogTemp, Warning, TEXT("slow added"));
			if (CurrentSlowsActive >= SlowsNeeded)
			{
				GetWorld()->GetTimerManager().ClearTimer(DeactivationTimer);
				Blizzard->Activate();
				UE_LOG(LogTemp, Warning, TEXT("Blizzard Activated"));	
			}

			FTimerHandle handle;
			triggerOwner->GetWorld()->GetTimerManager().SetTimer(handle, [triggerOwner, this](){
				CurrentSlowsActive--;
				UE_LOG(LogTemp, Warning, TEXT("slow removed"));
				if (CurrentSlowsActive == SlowsNeeded-1)
				{
					GetWorld()->GetTimerManager().SetTimer(DeactivationTimer, Blizzard, &ABlizzard::Deactivate, LingerDuration);
					UE_LOG(LogTemp, Warning, TEXT("Blizzard deactivates soon"));
				}}, slowEffect->Duration, false);
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
	bool Initialized = false;

};

//Damage Reduction Trigger
UCLASS(BlueprintType, EditInlineNew)
class UDamageReductionTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void OnTrigger(AWizardCharacter* triggerOwner, ABaseSpell* spell, ABaseCharacter* target) override
	{
		triggerOwner->SetDamageTakenMultiplier(triggerOwner->GetDamageTakenMultiplier() * Amount);
			
		FTimerHandle handle;
		triggerOwner->GetWorld()->GetTimerManager().SetTimer(handle, [triggerOwner, this]() {
			triggerOwner->SetDamageTakenMultiplier(triggerOwner->GetDamageTakenMultiplier() / Amount);
			}, Duration, false);
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
	void OnTrigger(AWizardCharacter* triggerOwner, ABaseSpell* spell, ABaseCharacter* target) override
	{
		//only execute if target died
		if (target == nullptr || !target->IsActorBeingDestroyed())
			return;

		if (FMath::FRand() > SpawnChance)
			return;

		AVortex* vortex = GetWorld()->SpawnActor<AVortex>(*VortexClass);
		vortex->InitSpell(target->GetActorLocation(), triggerOwner);
	};

private:
	UPROPERTY(EditDefaultsOnly)
	float SpawnChance = 0.3f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AVortex> VortexClass;
};
