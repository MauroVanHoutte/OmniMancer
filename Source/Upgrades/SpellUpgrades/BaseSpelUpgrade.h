// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffect.h"
#include "../TriggerHandlingComponent.h"
#include "BaseSpelUpgrade.generated.h"

UCLASS(Abstract, BlueprintType, EditInlineNew)
class UNREALPROJECT_API UBaseSpellUpgradeEffect : public UObject
{
	GENERATED_BODY()

public:
	virtual void ApplyToSpell(class ABaseSpell* Spell) {};
	virtual void OnUpgradeRegistered(class USpellUpgradesComponent* Component, TSubclassOf<ABaseSpell> Spell) {};
	virtual void OnUpgradeRemoved(class USpellUpgradesComponent* Component, TSubclassOf<ABaseSpell> Spell) {};
	virtual FFormatNamedArguments GetDescriptionArguments() { return FFormatNamedArguments(); };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Priority = 0;
	UPROPERTY(BlueprintReadWrite)
	int CurrentLevel = 0;
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class UNREALPROJECT_API UBPBaseSpellUpgrade : public UBaseSpellUpgradeEffect
{
	GENERATED_BODY()

public:
	virtual void ApplyToSpell(class ABaseSpell* Spell) override { BP_ApplyToSpell(Spell); };
	virtual void OnUpgradeRegistered(class USpellUpgradesComponent* Component, TSubclassOf<ABaseSpell> Spell) override { BP_OnUpgradeRegistered(Component, Spell); };
	virtual void OnUpgradeRemoved(class USpellUpgradesComponent* Component, TSubclassOf<ABaseSpell> Spell) override { BP_OnUpgradeRemoved(Component, Spell); };
	virtual FFormatNamedArguments GetDescriptionArguments() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ApplyToSpell(class ABaseSpell* Spell);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnUpgradeRegistered(class USpellUpgradesComponent* Component, TSubclassOf<ABaseSpell> Spell);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnUpgradeRemoved(class USpellUpgradesComponent* Component, TSubclassOf<ABaseSpell> Spell);
	UFUNCTION(BlueprintImplementableEvent)
	TMap<FString, FText> BP_GetDescriptionArguments();
};
