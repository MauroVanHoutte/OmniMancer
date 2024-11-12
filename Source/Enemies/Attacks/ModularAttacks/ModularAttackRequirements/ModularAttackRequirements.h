// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularAttackRequirements.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class UNREALPROJECT_API UModularAttackRequirementBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnBeginPlay(AActor* Owner) {};
	virtual void OnEndPlay() {};

	virtual bool AreRequirementsMet(const AActor* TargetActor) { return true; };
};
