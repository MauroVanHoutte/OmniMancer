// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterUpgrades.generated.h"

class AWizardCharacter;
class UBaseTriggerEffect;

UCLASS(BlueprintType, Abstract, Blueprintable, EditInlineNew)
class UNREALPROJECT_API UCharacterUpgrade : public UObject
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter* character) {};
	virtual void Remove(AWizardCharacter* character) {};
	virtual void SetTag(const FString& tag) { Tag = tag; };

protected:
	UPROPERTY(EditDefaultsOnly)
	FString Tag {};

};

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
