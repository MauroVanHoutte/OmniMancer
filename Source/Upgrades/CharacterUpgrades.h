// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffect.h"
#include "TriggerHandlingComponent.h"
#include "SpellCasting/Elements.h"
#include "CharacterUpgrades.generated.h"

//Upgrade Base
UCLASS(BlueprintType, Abstract, Blueprintable, EditInlineNew)
class UNREALPROJECT_API UCharacterUpgrade : public UObject
{
	GENERATED_BODY()
public:
	virtual void Apply(AActor* character) {};
	virtual void Remove(AActor* character) {};
	UFUNCTION(BlueprintCallable)
	virtual bool CanBeApplied();
	virtual void SetTag(const FString& tag) { Tag = tag; };
	UFUNCTION(BlueprintCallable)
	FText GetFormattedDescription() { return FText::Format(Description, GetDescriptionArguments()); };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly)
	FString Tag {};

	UPROPERTY(BlueprintReadWrite)
	int CurrentLevel = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxLevel = 1;

private:
	virtual FFormatNamedArguments GetDescriptionArguments() { return FFormatNamedArguments(); };
};

//Triggered Effects
UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API UTriggerUpgrade : public UCharacterUpgrade
{
	GENERATED_BODY()
public:
	virtual void Apply(AActor* character) override;
	virtual void Remove(AActor* character) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TriggerCondition Condition;
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBaseTriggerEffect* TriggerEffect;
};

//Stats
UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API UStatUpgrade : public UCharacterUpgrade
{
	GENERATED_BODY()
public:
	virtual void Apply(AActor* character) override;
	virtual void Remove(AActor* character) override;

private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBaseStatBoost* StatBoost;
};

//Cooldown upgrade
UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API UCooldownMultiplierUpgrade : public UCharacterUpgrade
{
	GENERATED_BODY()
public:
	virtual void Apply(AActor* character) override;
	virtual void Remove(AActor* character) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float Multiplier = 0.8f;
	UPROPERTY(EditDefaultsOnly)
	bool bSpellMultiplier = true;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bSpellMultiplier"))
	TSubclassOf<class ABaseSpell> SpellType;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!bSpellMultiplier"))
	WizardElement Element = WizardElement::None;
};

//Repeating effect
UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API URepeatingEffectUpgrade : public UCharacterUpgrade
{
	GENERATED_BODY()
public:
	virtual void Apply(AActor* character) override;
	virtual void Remove(AActor* character) override;
private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBaseRepeatingEffect* Effect;
};

UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API UShieldUpgrade : public UCharacterUpgrade
{
	GENERATED_BODY()

public:
	virtual void Apply(AActor* character) override;
	virtual void Remove(AActor* character) override;

protected:
	virtual FFormatNamedArguments GetDescriptionArguments();

	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBaseHealthComponent* ShieldComponent;
	UPROPERTY(EditDefaultsOnly)
	float ShieldHealthPerLevel = 10;
	UPROPERTY(EditDefaultsOnly)
	float BaseShieldHealth = 0.f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UWidgetComponent> HealthbarComponentClass;
	class UWidgetComponent* InstancedComponent;
};

UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API UKillBarrierUpgrade : public UShieldUpgrade
{
	GENERATED_BODY()

public:
	virtual void Apply(AActor* character) override;
	virtual void Remove(AActor* character) override;

private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UHealHealthCompTrigger* Trigger;
};