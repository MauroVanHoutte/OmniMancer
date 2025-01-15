// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BaseCombatEncounterManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEncounterCompletedSignature);

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALPROJECT_API UBaseCombatEncounterManager : public USceneComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void StartEncounter() { bIsInProgress = true; };
	UFUNCTION(BlueprintCallable)
	virtual bool IsEncounterInProgress() { return bIsInProgress; };
	
	UPROPERTY(BlueprintAssignable)
	FEncounterCompletedSignature OnEncounterCompletedDelegate;
	
protected:
	virtual void EncounterCompleted()
	{
		bIsInProgress = false;
		OnEncounterCompletedDelegate.Broadcast();
	};

	bool bIsInProgress = false;
};
