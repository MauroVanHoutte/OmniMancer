// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpelUpgrade.h"
#include "SpellRecast.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNREALPROJECT_API USpellRecast : public UBaseSpellUpgradeEffect
{
	GENERATED_BODY()
public:
	virtual void ApplyToSpell(class ABaseSpell* Spell) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UFUNCTION()
	void OnSpellDestroyed(AActor* DestroyedActor);

	UPROPERTY(Transient)
	TMap<AActor*, float> RecastCounters;

	UPROPERTY(EditDefaultsOnly)
	float SizeMultiplier = 1.75f;
	UPROPERTY(EditDefaultsOnly)
	float DamageMultiplier = 0.5f;
};
