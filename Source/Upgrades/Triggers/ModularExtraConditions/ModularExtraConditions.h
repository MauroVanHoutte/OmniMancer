// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpellCasting/Elements.h"
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
	virtual FFormatNamedArguments GetDescriptionArguments() { return FFormatNamedArguments(); };
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
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	
	UPROPERTY(EditDefaultsOnly)
	CompoundConditionOperation Operation;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UModularExtraConditionsBase*> Conditions;
};

UCLASS(BlueprintType, EditInlineNew)
class USpellCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// AllowedSpell Description arguments are named AllowedSpellN starting at 0
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class ABaseSpell>> AllowedSpellTypes;
};

UCLASS(BlueprintType, EditInlineNew)
class USpellStatusEffectCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description argument is named RequiredSpellStatusEffect
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
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description argument is named CooldownTime
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
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description argument is named RequiredAffectingStatusEffect
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBaseStatusEffect> RequiredStatusEffectType;
};

UCLASS(BlueprintType, EditInlineNew)
class UTargetInRangeCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target);
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description argument is named MaximumDistance
	UPROPERTY(EditDefaultsOnly)
	float Range = 700.f;
};

UCLASS(BlueprintType, EditInlineNew)
class URandomChanceCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target);
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Percentage, Description argument is named ActivationChance
	UPROPERTY(EditDefaultsOnly)
	float Chance = 70;
};

UCLASS(BlueprintType, EditInlineNew)
class UNthActivationCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target);
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	//Condition is satisfied every N activations, Description argument is named ActivationInterval
	UPROPERTY(EditDefaultsOnly)
	int ActivationInterval = 4;
	int ActivatonCount = 0;
};

UCLASS(BlueprintType, EditInlineNew)
class USpellElementCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target);
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description argument is named SpellElement
	UPROPERTY(EditDefaultsOnly)
	WizardElement Element;
};

UCLASS(BlueprintType, EditInlineNew)
class ULatestElementCondition : public UModularExtraConditionsBase
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(class AActor* triggerOwner, class ABaseSpell* spell, class AActor* target);
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description argument is named LatestElement
	UPROPERTY(EditDefaultsOnly)
	WizardElement Element = WizardElement::Fire;
};