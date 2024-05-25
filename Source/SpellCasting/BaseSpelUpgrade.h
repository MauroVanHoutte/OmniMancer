// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseSpelUpgrade.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALPROJECT_API UBaseSpellUpgrade : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void ApplyToSpell(class ABaseSpell* Spell) {};

	int GetPriority() const { return Priority; };

private:
	UPROPERTY(EditDefaultsOnly)
	int Priority = 0;
};
