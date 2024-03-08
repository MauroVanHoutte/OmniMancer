// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularExtraConditions.generated.h"

UENUM(BlueprintType)
enum class CompoundConditionOperation
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
	TSubclassOf<class ABaseSpell> RequiredSpell;
};