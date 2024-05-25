// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellCasting/BaseSpelUpgrade.h"
#include "BPBaseSpellUpgrade.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class UNREALPROJECT_API UBPBaseSpellUpgrade : public UBaseSpellUpgrade
{
	GENERATED_BODY()
	
public:
	virtual void ApplyToSpell(class ABaseSpell* Spell) override { BP_ApplyToSpell(Spell); };

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ApplyToSpell(class ABaseSpell* Spell);
};
