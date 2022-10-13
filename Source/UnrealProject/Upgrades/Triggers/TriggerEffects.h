// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../WizardCharacter.h"
#include "../../Spells/BaseSpell.h"
#include "../../Spells/BaseProjectile.h"
#include "../../Spells/Blizzard.h"
#include <Kismet/KismetSystemLibrary.h>
#include "TriggerEffects.generated.h"

UCLASS(Abstract, EditInlineNew)
class UBaseTriggerEffect : public UObject
{
	GENERATED_BODY()
public:

	virtual void OnTrigger(AWizardCharacter* caster, ABaseSpell* spell, AActor* target) {};
	FString UpgradeTag{};
	UPROPERTY(EditAnywhere)
	TriggerCondition Condition{};
	virtual void SetVars(const TMap<FString, float>& variables) {};
protected:
	bool TargetKilled(AActor* actor)
	{
		if (actor == nullptr || actor->IsActorBeingDestroyed())
			return true;
		return false;
	}
};

//Slow enemies in radius around caster
UCLASS(BlueprintType, EditInlineNew)
class UAoeSlowTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void OnTrigger(AWizardCharacter* caster, ABaseSpell* spell, AActor* target) override
	{
		TArray<AActor*> ignore{ caster };
		TArray<AActor*> outActors;
		UKismetSystemLibrary::SphereOverlapActors(caster->GetWorld(), caster->GetActorLocation(), Radius, TArray<TEnumAsByte<EObjectTypeQuery>>(), ABaseCharacter::StaticClass(), ignore, outActors);
		for (AActor* actor : outActors)
		{
			auto enemy = Cast<ABaseCharacter>(actor);
			if (enemy->IsValidLowLevel())
				enemy->AddStatusEffect(FStatusEffect{ Type::Slow, -1, Slow , Duration, caster });
		}
	};

	void SetVars(const TMap<FString, float>& variables) override
	{
		if (variables.Contains("Radius"))
			Radius = variables["Radius"];
		if (variables.Contains("Slow"))
			Slow = variables["Slow"];
		if (variables.Contains("Duration"))
			Duration = variables["Duration"];
	};

	void SetVars(float radius, float slow, float duration)
	{
		Radius = radius;
		Slow = slow;
		Duration = duration;
	};

private:
	UPROPERTY(EditAnywhere)
	float Radius = 600;
	UPROPERTY(EditAnywhere)
	float Slow = 20;
	UPROPERTY(EditAnywhere)
	float Duration = 3;
};

//Give the caster a speed boost
UCLASS(BlueprintType, EditInlineNew)
class USpeedBuffTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void OnTrigger(AWizardCharacter* caster, ABaseSpell* spell, AActor* target) override
	{
		caster->AddStatusEffect(FStatusEffect{ Type::Slow, -1, -Speed, Duration, caster });
	};

	void SetVars(const TMap<FString, float>& variables) override
	{
		if (variables.Contains("Speed"))
			Speed = variables["Speed"];
		if (variables.Contains("Duration"))
			Duration = variables["Duration"];
	};

	void SetVars(float speed, float duration)
	{
		Speed = speed;
		Duration = duration;
	};

private:
	UPROPERTY(EditAnywhere)
	float Speed = 20;
	UPROPERTY(EditAnywhere)
	float Duration = 3;
};

//Chance to lower Cooldowns
UCLASS(BlueprintType, EditInlineNew)
class UBaseAttackLowerCooldownTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void OnTrigger(AWizardCharacter* caster, ABaseSpell* spell, AActor* target) override
	{
		//only execute if base projectile
		auto baseProjectile = Cast<ABaseProjectile>(spell);
		if (baseProjectile == nullptr)
			return;

		float rand = FMath::FRandRange(0, 1);
		if (rand < Chance)
			caster->LowerCooldowns(Amount);
	};

	void SetVars(const TMap<FString, float>& variables) override
	{
		if (variables.Contains("Chance"))
			Chance = variables["Chance"];
		if (variables.Contains("Amount"))
			Amount = variables["Amount"];
	};

	void SetVars(int chance, float amount)
	{
		Chance = chance;
		Amount = amount;
	};

private:
	UPROPERTY(EditAnywhere)
	float Chance = 0.3f;
	UPROPERTY(EditAnywhere)
	float Amount = 1.f;
};

//Temp Fire Rate
UCLASS(BlueprintType, EditInlineNew)
class UBaseAttackTempFireRateTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void OnTrigger(AWizardCharacter* caster, ABaseSpell* spell, AActor* target) override
	{
		//only execute if base projectile
		auto baseProjectile = Cast<ABaseProjectile>(spell);
		if (baseProjectile == nullptr)
			return;

		float currentCooldown = caster->GetBasicAttackCooldown();
		caster->SetBasicAttackCooldown(currentCooldown / FireRateMultiplier);

		//undo buff after duration
		FTimerHandle handle;
		caster->GetWorld()->GetTimerManager().SetTimer(handle, [caster, this]() 
			{
				float currentCooldown = caster->GetBasicAttackCooldown();
				caster->SetBasicAttackCooldown(currentCooldown * FireRateMultiplier);
			}, Duration, false);
	};

	void SetVars(const TMap<FString, float>& variables) override
	{
		if (variables.Contains("FireRateMultiplier"))
			FireRateMultiplier = variables["FireRateMultiplier"];
		if (variables.Contains("Duration"))
			Duration = variables["Duration"];
	};

	void SetVars(float fireRateMultiplier, float duration)
	{
		FireRateMultiplier = fireRateMultiplier;
		Duration = duration;
	};

private:
	UPROPERTY(EditAnywhere)
	float FireRateMultiplier = 1.1f;
	UPROPERTY(EditAnywhere)
	float Duration = 3.f;

};

//Temp Spell Damage on Kill
UCLASS(BlueprintType, EditInlineNew)
class USpellDamageOnKillTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void OnTrigger(AWizardCharacter* caster, ABaseSpell* spell, AActor* target) override
	{
		//only execute if target died
		if (target == nullptr || !target->IsActorBeingDestroyed())
			return;

		float currentSpellDamageMultiplier = caster->GetSpellDamageMultiplier();
		caster->SetSpellDamageMultiplier(currentSpellDamageMultiplier * (1+SpellDamageBoost));

		//undo buff after duration
		FTimerHandle handle;
		caster->GetWorld()->GetTimerManager().SetTimer(handle, [caster, this]()
			{
				float currentSpellDamageMultiplier = caster->GetSpellDamageMultiplier();
				caster->SetSpellDamageMultiplier(currentSpellDamageMultiplier / (1+SpellDamageBoost));
			}, Duration, false);
	};

	void SetVars(const TMap<FString, float>& variables) override
	{
		if (variables.Contains("SpellDamageBoost"))
			SpellDamageBoost = variables["SpellDamageBoost"];
		if (variables.Contains("Duration"))
			Duration = variables["Duration"];
	};

	void SetVars(float spellDamageBoost, float duration)
	{
		SpellDamageBoost = spellDamageBoost;
		Duration = duration;
	};

private:
	UPROPERTY(EditAnywhere)
	float SpellDamageBoost = 0.15f;
	UPROPERTY(EditAnywhere)
	float Duration = 7.f;
};

//Blizzard Trigger Effect
UCLASS(BlueprintType, EditInlineNew)
class UBlizzardOnSlowTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void OnTrigger(AWizardCharacter* caster, ABaseSpell* spell, AActor* target) override
	{
		if (spell == nullptr)
			return;

		Blizzard->SetWizard(caster);

		auto slowEffect = spell->GetStatusEffects().FindByPredicate([](const FStatusEffect& effect) {return effect.EffectType == Type::Slow; });
		if (slowEffect != nullptr)
		{
			CurrentSlowsActive++;
			UE_LOG(LogTemp, Warning, TEXT("slow added"));
			if (CurrentSlowsActive >= SlowsNeeded)
			{
				Blizzard->Activate();
				UE_LOG(LogTemp, Warning, TEXT("Blizzard Activated"));	
			}

			FTimerHandle handle;
			caster->GetWorld()->GetTimerManager().SetTimer(handle, [caster, this](){
				CurrentSlowsActive--;
				UE_LOG(LogTemp, Warning, TEXT("slow removed"));
				if (CurrentSlowsActive < SlowsNeeded)
				{
					Blizzard->Deactivate();
					UE_LOG(LogTemp, Warning, TEXT("Blizzard Deactivated"));
				}}, slowEffect->Duration, false);
		}
	};

	void SetVars(const TMap<FString, float>& variables) override
	{
		if (variables.Contains("SlowsNeeded"))
			SlowsNeeded = (int)variables["SlowsNeeded"];
	};

private:
	int CurrentSlowsActive = 0;
	UPROPERTY(EditAnywhere)
	int SlowsNeeded = 4;
	UPROPERTY(EditAnywhere, Instanced)
	ABlizzard* Blizzard;

};

//Damage Reduction Trigger
UCLASS(BlueprintType, EditInlineNew)
class UDamageReductionTrigger : public UBaseTriggerEffect
{
	GENERATED_BODY()
public:
	void OnTrigger(AWizardCharacter* caster, ABaseSpell* spell, AActor* target) override
	{
		caster->SetDamageTakenMultiplier(caster->GetDamageTakenMultiplier() * (1 - Amount));
			
		FTimerHandle handle;
		caster->GetWorld()->GetTimerManager().SetTimer(handle, [caster, this]() {
			caster->SetDamageTakenMultiplier(caster->GetDamageTakenMultiplier() / (1 - Amount));
			}, Duration, false);
	};

	void SetVars(const TMap<FString, float>& variables) override
	{
		if (variables.Contains("Duration"))
			Duration = variables["Duration"];
		if (variables.Contains("Amount"))
			Amount = variables["Amount"];
	};

private:
	UPROPERTY(EditAnywhere)
	float Duration = 3.f;
	UPROPERTY(EditAnywhere)
	float Amount = 0.15f;
};


