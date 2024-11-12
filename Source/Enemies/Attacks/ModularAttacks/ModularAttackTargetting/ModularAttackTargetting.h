// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularAttackTargetting.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class UNREALPROJECT_API UModularAttackTargettingBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnBeginPlay(AActor* Owner) { OwningActor = Owner; };
	virtual void OnEndPlay() {};
	virtual void GatherTargets(const FVector& TargetLocation, TArray<AActor*>& outTargetActors, TArray<FVector>& outTargetLocations) {};

protected:
	UPROPERTY(Transient)
	AActor* OwningActor;
};
