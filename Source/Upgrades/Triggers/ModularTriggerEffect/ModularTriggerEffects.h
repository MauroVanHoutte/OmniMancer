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

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class UStatusEffectTriggerEffect : public UModularTriggerEffectBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteEffect(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) override;

private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBaseStatusEffect* StatusEffect;
};
