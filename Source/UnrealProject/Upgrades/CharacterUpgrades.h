// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffect.h"
#include "CharacterUpgrades.generated.h"

class AWizardCharacter;
class UBaseTriggerEffect;
class UBaseStatBoost;
class UBaseAttackStatusEffect;
class UBaseRepeatingEffect;
//Upgrade Base
UCLASS(BlueprintType, Abstract, Blueprintable, EditInlineNew)
class UNREALPROJECT_API UCharacterUpgrade : public UObject
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter* character) {};
	virtual void Remove(AWizardCharacter* character) {};
	virtual void SetTag(const FString& tag) { Tag = tag; };

	UPROPERTY(EditDefaultsOnly)
	FString Tag {};

protected:
	UPROPERTY(VisibleAnywhere)
	bool Applied = false;

};

//Triggered Effects
UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API UTriggerUpgrade : public UCharacterUpgrade
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter* character) override;
	virtual void Remove(AWizardCharacter* character) override;

private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	UBaseTriggerEffect* TriggerEffect;
};

//Stats
UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API UStatUpgrade : public UCharacterUpgrade
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter* character) override;
	virtual void Remove(AWizardCharacter* character) override;

private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	UBaseStatBoost* StatBoost;
};

//Base attack status effect
UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API UBaseAttackStatusEffectUpgrade : public UCharacterUpgrade
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter* character) override;
	virtual void Remove(AWizardCharacter* character) override;

private:
	//UPROPERTY(EditDefaultsOnly)
	//FStatusEffect StatusEffect;
};

//Reflect status effect
UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API UReflectStatusEffectUpgrade : public UCharacterUpgrade
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter* character) override;
	virtual void Remove(AWizardCharacter* character) override;

private:
	//UPROPERTY(EditDefaultsOnly)
	//FStatusEffect StatusEffect;
};

//Repeating effect
UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API URepeatingEffectUpgrade : public UCharacterUpgrade
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter* character) override;
	virtual void Remove(AWizardCharacter* character) override;
private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	UBaseRepeatingEffect* Effect;
};
