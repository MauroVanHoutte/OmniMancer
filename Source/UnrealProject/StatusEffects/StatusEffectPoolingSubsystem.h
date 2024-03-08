// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StatusEffectPoolingSubsystem.generated.h"

USTRUCT()
struct FPooledStatusEffect
{
    GENERATED_BODY()
    UPROPERTY()
    class UBaseStatusEffect* StatusEffect;
    bool bInUse;
};

USTRUCT()
struct FPooledStatusEffectsArray
{
    GENERATED_BODY()
    UPROPERTY()
    TArray<FPooledStatusEffect> StatusEffectsArray;
};

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UStatusEffectPoolingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
    // Begin USubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // End USubsystem

    class UBaseStatusEffect* GetStatusEffectInstance(class UBaseStatusEffect* StatusEffect);
    void ReturnStatusEffectInstance(const class UBaseStatusEffect* StatusEffect);

private:
    UPROPERTY()
    TMap<UClass*, FPooledStatusEffectsArray> StatusEffects;
};