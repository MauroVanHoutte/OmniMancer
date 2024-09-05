// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularTriggerEffects.generated.h"

UCLASS(Abstract, EditInlineNew)
class UModularTriggerEffectBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) {};
};

UCLASS(Abstract, Blueprintable, EditInlineNew)
class UBPModularTriggerEffectBase : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override
	{
		BP_ExecuteEffect(targetLocations, targetActors, Damage, instigator);
	};

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ExecuteEffect(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator);
};

UCLASS(BlueprintType, EditInlineNew)
class UStatusEffectTriggerEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;

private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBaseStatusEffect* StatusEffect;
};

UCLASS(BlueprintType, EditInlineNew)
class URadialProjectilesTriggerEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseProjectile> ProjectileType;
	UPROPERTY(EditDefaultsOnly)
	int ProjectileCount = 5;
};

UCLASS(BlueprintType, EditInlineNew)
class UHealTargets : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float HealAmount = 5.f;
};