// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../WizardCharacter.h"
#include "StatBoosts.generated.h"

/**
 *
 */
UCLASS(Abstract, EditInlineNew)
class UNREALPROJECT_API UBaseStatBoost : public UObject
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter* character) {};
	virtual void Remove(AWizardCharacter* character) {};
};

UCLASS(EditInlineNew, BlueprintType)
class UNREALPROJECT_API UBurnDurationBoost : public UBaseStatBoost
{
	GENERATED_BODY()
public:
	void Apply(AWizardCharacter* character) override
	{
		character->SetBurnDurationMultiplier(character->GetBurnDurationMultiplier() + MultiplierIncrease);
	};
	void Remove(AWizardCharacter* character) override
	{
		character->SetBurnDurationMultiplier(character->GetBurnDurationMultiplier() - MultiplierIncrease);
	};

	UPROPERTY(EditAnywhere)
	float MultiplierIncrease = 0.5f;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREALPROJECT_API USlowDurationBoost : public UBaseStatBoost
{
	GENERATED_BODY()
public:
	void Apply(AWizardCharacter* character) override
	{
		character->SetSlowDurationMultiplier(character->GetSlowDurationMultiplier() + MultiplierIncrease);
	};
	void Remove(AWizardCharacter* character) override
	{
		character->SetSlowDurationMultiplier(character->GetSlowDurationMultiplier() - MultiplierIncrease);
	};

	UPROPERTY(EditAnywhere)
	float MultiplierIncrease = 0.5f;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREALPROJECT_API UStunDurationBoost : public UBaseStatBoost
{
	GENERATED_BODY()
public:
	void Apply(AWizardCharacter* character) override
	{
		character->SetStunDurationMultiplier(character->GetStunDurationMultiplier() + MultiplierIncrease);
	};
	void Remove(AWizardCharacter* character) override
	{
		character->SetStunDurationMultiplier(character->GetStunDurationMultiplier() - MultiplierIncrease);
	};

	UPROPERTY(EditAnywhere)
	float MultiplierIncrease = 0.5f;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREALPROJECT_API UDashDistanceBoost : public UBaseStatBoost
{
	GENERATED_BODY()
public:
	void Apply(AWizardCharacter* character) override
	{
		character->SetDashForce(character->GetDashForce() * Multiplier);
	};
	void Remove(AWizardCharacter* character) override
	{
		character->SetDashForce(character->GetDashForce() / Multiplier);
	};

	UPROPERTY(EditAnywhere)
	float Multiplier = 1.5f;
};