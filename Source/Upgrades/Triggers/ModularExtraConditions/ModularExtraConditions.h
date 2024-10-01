// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularExtraConditions.generated.h"

UENUM(BlueprintType)
enum class CompoundConditionOperation : uint8
{
	AND,
	OR
};

/**
 * 
 */
UCLASS(Abstract, EditInlineNew)
class UModularExtraConditionsBase : public UObject
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target) { return true; };
	virtual void OnExecution(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator) {};
};

UCLASS(Abstract, Blueprintable, EditInlineNew)
class UBPModularExtraConditionBase : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target) override { return BP_CheckCondition(triggerOwner, spell, target); };

protected:
	UFUNCTION(BlueprintImplementableEvent)
	bool BP_CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target);
};

UCLASS(BlueprintType, EditInlineNew)
class UCompoundModularCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target) override;
	virtual void OnExecution(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator);

private:
	
	UPROPERTY(EditDefaultsOnly)
	CompoundConditionOperation Operation;

	UPROPERTY(EditDefaultsOnly, Instanced)
	UModularExtraConditionsBase* ConditionOne;
	UPROPERTY(EditDefaultsOnly, Instanced)
	UModularExtraConditionsBase* ConditionTwo;
};

UCLASS(BlueprintType, EditInlineNew)
class USpellCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class ABaseSpell>> AllowedSpellTypes;
};

UCLASS(BlueprintType, EditInlineNew)
class USpellStatusEffectCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBaseStatusEffect> RequiredStatusEffect;
};

UCLASS(BlueprintType, EditInlineNew)
class UCooldownCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target);
	virtual void OnExecution(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, class APawn* instigator);

private:
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 5;
	FTimerHandle TimerHandle;
};

UCLASS(BlueprintType, EditInlineNew)
class UTargetAffectedByStatusEffectCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target);

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBaseStatusEffect> RequiredStatusEffectType;
};

UCLASS(BlueprintType, EditInlineNew)
class UTargetInRangeCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target);

private:
	UPROPERTY(EditDefaultsOnly)
	float Range = 700.f;
};