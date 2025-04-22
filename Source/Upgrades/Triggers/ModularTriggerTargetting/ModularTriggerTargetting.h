// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularTriggerTargetting.generated.h"

UCLASS(Abstract, BlueprintType, EditInlineNew)
class UModularTriggerTargettingBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) {};
	virtual FFormatNamedArguments GetDescriptionArguments() { return FFormatNamedArguments(); };
};

UCLASS(Abstract, Blueprintable)
class UBPModularTriggerTargettingBase : public UModularTriggerTargettingBase
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors);
};

UCLASS()
class UTargetHitActor : public UModularTriggerTargettingBase
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) override;
};

UCLASS()
class UTargetHitActorLocation : public UModularTriggerTargettingBase
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) override;
};

UCLASS()
class UTargetActorsProximateToTarget : public UModularTriggerTargettingBase
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description argument is named RadiusAroundTarget
	UPROPERTY(EditDefaultsOnly)
	float Radius = 400.f;
	UPROPERTY(EditDefaultsOnly)
	bool bCheckAffiliation = true;
};

UCLASS()
class UTargetActorsProximateToOwner : public UModularTriggerTargettingBase
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) override;
	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description argument is named RadiusAroundCaster
	UPROPERTY(EditDefaultsOnly)
	float Radius = 400.f;
	UPROPERTY(EditDefaultsOnly)
	bool bCheckAffiliation = true;
};

UCLASS()
class UTargetHitActors : public UModularTriggerTargettingBase
{
	GENERATED_BODY()
public:
	virtual void GatherTargets(class AActor* inTriggerOwner, class ABaseSpell* inSpell, class AActor* inTarget, TArray<FVector>& outTargetLocations, TArray<class AActor*>& outTargetActors) override;
};

