// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Containers/Queue.h>
#include "UObject/NoExportTypes.h"
#include "ModularTriggerEffects.generated.h"

UCLASS(Abstract, BlueprintType, EditInlineNew)
class UModularTriggerEffectBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) {};
	virtual FFormatNamedArguments GetDescriptionArguments() { return FFormatNamedArguments(); };
};

UCLASS(Abstract, Blueprintable)
class UBPModularTriggerEffectBase : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override
	{
		BP_ExecuteEffect(TriggeringSpell, targetLocations, targetActors, Damage, instigator);
	};

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator);
};

UCLASS()
class UStatusEffectTriggerEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBaseStatusEffect* StatusEffect;
};

UCLASS()
class URadialProjectilesTriggerEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseProjectile> ProjectileType;
	// Description Argument is named ProjectileCount
	UPROPERTY(EditDefaultsOnly)
	int ProjectileCount = 5;
};

UCLASS()
class UHealTargets : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description Argument is named HealAmount
	UPROPERTY(EditDefaultsOnly)
	float HealAmount = 5.f;
};

UCLASS()
class UDamagePerHitActorOnSpellEndTriggerEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UFUNCTION()
	void OnSpellDestroyed(class ABaseSpell* Spell);
	// Description Argument is named DamagePerTarget
	UPROPERTY(EditDefaultsOnly)
	float DamagePerTarget = 3.f;
};

UCLASS()
class UCastSpellTriggerEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description Argument is named SpellToCast
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseSpell> SpellToCast;
	UPROPERTY(EditDefaultsOnly)
	bool bSendSpellEvents = false;
	//If false the new spell is spawned at the triggering spells location
	UPROPERTY(EditDefaultsOnly)
	bool bSpawnSpellAtInstigator = false;
};

UCLASS()
class UCastOnSpellDestroyedTriggerEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UFUNCTION()
	void CastSpell(class ABaseSpell* Spell);

	// Description Argument is named SpellToCast
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseSpell> SpellToCast;
	UPROPERTY(EditDefaultsOnly)
	bool bSendSpellEvents = false;
	//If false the new spell is spawned at the triggering spells location
	UPROPERTY(EditDefaultsOnly)
	bool bSpawnSpellAtInstigator = false;
	UPROPERTY(Transient)
	class APawn* Instigator;
	TMap<const class ABaseSpell*, TArray<FVector>> TargetLocations;
};

UCLASS()
class URecastDamageSizeMulTriggerEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UFUNCTION()
	void CastSpell(class ABaseSpell* Spell);
	// Description Argument is named RecastCount
	UPROPERTY(EditDefaultsOnly)
	int RecastCount;
	UPROPERTY(EditDefaultsOnly)
	bool bSendSpellEvents = false;
	// Description Argument is named SizeMultiplier
	UPROPERTY(EditDefaultsOnly)
	float SizeMultiplier = 1.75f;
	// Description Argument is named DamageMultiplier
	UPROPERTY(EditDefaultsOnly)
	float DamageMultiplier = 0.5f;
	UPROPERTY(Transient)
	class APawn* Instigator;
	TMap<AActor*, int> RecastTracker;
};

UCLASS()
class UAddBouncesTriggerEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description Argument is named ExtraBounces
	UPROPERTY(EditDefaultsOnly)
	int ExtraBounces = 1;
};

UCLASS()
class USummonTriggerEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description Argument is SummonedClass
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APooledActor> SummonedClass;
	// Description Argument is SummonLimit
	UPROPERTY(EditDefaultsOnly)
	int SummonLimit = 5;
	int QueueSize = 0;
	UPROPERTY(Transient)
	TArray<AActor*> SummonQueue;
};

UCLASS()
class UReduceCooldownsForActiveElementsEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(class ABaseSpell* TriggeringSpell, const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description Argument is CooldownReductionAmount
	UPROPERTY(EditDefaultsOnly)
	float ReductionAmount = 0.3f;
};