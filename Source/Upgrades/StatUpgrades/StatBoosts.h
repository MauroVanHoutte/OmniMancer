// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WizardCharacter.h"
#include "StatBoosts.generated.h"

/**
 *
 */
UCLASS(Abstract, EditInlineNew)
class UNREALPROJECT_API UBaseStatBoost : public UObject
{
	GENERATED_BODY()
public:
	virtual void Apply(AActor* character);
	virtual void Remove(AActor* character) {};

	virtual FFormatNamedArguments GetDescriptionArguments() { return FFormatNamedArguments(); };

protected:
	UPROPERTY(Transient)
	class UStatComponent* StatComponent;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREALPROJECT_API USpellCooldownBoost : public UBaseStatBoost
{
	GENERATED_BODY()
public:
	void Apply(AActor* character) override;
	void Remove(AActor* character) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float CooldownMultiplier = 0.5f;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREALPROJECT_API UElementSpellCooldownBoost : public UBaseStatBoost
{
	GENERATED_BODY()
public:
	void Apply(AActor* character) override;
	void Remove(AActor* character) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UPROPERTY(EditDefaultsOnly)
	WizardElement Element = WizardElement::None;
	UPROPERTY(EditDefaultsOnly)
	float CooldownMultiplier = 0.5f;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREALPROJECT_API USpellDamageBoost : public UBaseStatBoost
{
	GENERATED_BODY()
public:
	void Apply(AActor* character) override;
	void Remove(AActor* character) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float DamageMultiplier = 1.5f;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREALPROJECT_API UElementSpellDamageBoost : public UBaseStatBoost
{
	GENERATED_BODY()
public:
	void Apply(AActor* character) override;
	void Remove(AActor* character) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UPROPERTY(EditDefaultsOnly)
	WizardElement Element = WizardElement::None;
	UPROPERTY(EditDefaultsOnly)
	float DamageMultiplier = 1.5f;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREALPROJECT_API UBasicAttackDamageBoost : public UBaseStatBoost
{
	GENERATED_BODY()
public:
	void Apply(AActor* character) override;
	void Remove(AActor* character) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float DamageMultiplier = 1.5f;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREALPROJECT_API UBasicAttackCooldownBoost : public UBaseStatBoost
{
	GENERATED_BODY()
public:
	void Apply(AActor* character) override;
	void Remove(AActor* character) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float CooldownMultiplier = 0.5f;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREALPROJECT_API UExtraBouncesBoost : public UBaseStatBoost
{
	GENERATED_BODY()
public:
	void Apply(AActor* character) override;
	void Remove(AActor* character) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UPROPERTY(EditDefaultsOnly)
	int ExtraBounces = 1;
};