// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Kismet/KismetSystemLibrary.h>
#include "MoveRequirements.generated.h"

UCLASS(EditInlineNew)
class UNREALPROJECT_API UBaseMoveRequirement : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual bool AreRequirementsMet(AActor* Owner, AActor* Target) { return true; };
	virtual void OnAttackExecuted(AActor* Owner, AActor* Target) {};
};

UCLASS()
class UNREALPROJECT_API UHealthMoveRequirement : public UBaseMoveRequirement
{
	GENERATED_BODY()

public:
	virtual bool AreRequirementsMet(AActor* Owner, AActor* Target) override;

private:
	//If false check target hp
	UPROPERTY(EditDefaultsOnly)
	bool bCheckOwnerHealth = true;
	UPROPERTY(EditDefaultsOnly)
	bool bRequireHealthBelowThreshold = true;
	UPROPERTY(EditDefaultsOnly)
	bool bUsePercentageThreshold = true;
	UPROPERTY(EditDefaultsOnly)
	float Threshold = 0.5f;
};

UCLASS()
class UNREALPROJECT_API ULimitedUsesRequirement : public UBaseMoveRequirement
{
	GENERATED_BODY()

public:
	virtual bool AreRequirementsMet(AActor* Owner, AActor* Target) override;
	virtual void OnAttackExecuted(AActor* Owner, AActor* Target) override;

private:
	UPROPERTY(EditDefaultsOnly)
	int NumberOfUses = 1;
	int UseCounter = 0;
};

UCLASS()
class UNREALPROJECT_API URangeRequirement : public UBaseMoveRequirement
{
	GENERATED_BODY()

public:
	virtual bool AreRequirementsMet(AActor* Owner, AActor* Target) override;

private:
	//If true target needs to be closer than range if false target needs to be further
	UPROPERTY(EditDefaultsOnly)
	bool bRequireCloserThanThreshold = true;
	UPROPERTY(EditDefaultsOnly)
	float RangeThreshold = 900;
};

UCLASS()
class UNREALPROJECT_API UCooldownRequirement : public UBaseMoveRequirement
{
	GENERATED_BODY()

public:
	virtual bool AreRequirementsMet(AActor* Owner, AActor* Target);
	virtual void OnAttackExecuted(AActor* Owner, AActor* Target);

private:
	UPROPERTY(EditDefaultsOnly)
	float CooldownDuration = 10.f;
	FTimerHandle TimerHandle;
};

UCLASS()
class UNREALPROJECT_API USphereTraceRequirement : public UBaseMoveRequirement
{
	GENERATED_BODY()

public:
	virtual bool AreRequirementsMet(AActor* Owner, AActor* Target);
	
private:
	UPROPERTY(EditAnywhere)
	float TraceLength = 500.f;
	UPROPERTY(EditAnywhere)
	float TraceRadius = 75.f;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_GameTraceChannel4;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DebugDraw = EDrawDebugTrace::ForDuration;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DebugDraw == EDrawDebugTrace::ForDuration"))
	float DebugDrawDuration = 2.f;
};