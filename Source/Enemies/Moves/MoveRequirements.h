// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MoveRequirements.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew)
class UNREALPROJECT_API UBaseMoveRequirement : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual bool AreRequirementsMet(AActor* Owner, AActor* Target) { return true; };
};
