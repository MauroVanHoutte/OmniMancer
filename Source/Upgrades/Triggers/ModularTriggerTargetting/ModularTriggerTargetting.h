// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularTriggerTargetting.generated.h"

UCLASS(Abstract, EditInlineNew)
class UModularTriggerTargettingBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) {};
};

UCLASS(Abstract, Blueprintable, EditInlineNew)
class UBPModularTriggerTargettingBase : public UModularTriggerTargettingBase
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors);
};

UCLASS(BlueprintType, EditInlineNew)
class UTargetHitActor : public UModularTriggerTargettingBase
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) override;
};

UCLASS(BlueprintType, EditInlineNew)
class UTargetHitActorLocation : public UModularTriggerTargettingBase
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) override;
};

UCLASS(BlueprintType, EditInlineNew)
class UTargetActorsProximateToTarget : public UModularTriggerTargettingBase
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float Radius = 400.f;
	UPROPERTY(EditDefaultsOnly)
	bool bCheckAffiliation = true;
};

UCLASS(BlueprintType, EditInlineNew)
class UTargetActorsProximateToOwner : public UModularTriggerTargettingBase
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float Radius = 400.f;
	UPROPERTY(EditDefaultsOnly)
	bool bCheckAffiliation = true;
};