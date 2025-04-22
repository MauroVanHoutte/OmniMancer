// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectPoolingSubsystem.h"
#include "StatusEffects/StatusEffect.h"

void UStatusEffectPoolingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UStatusEffectPoolingSubsystem::Deinitialize()
{
    StatusEffects.Empty();
}

inline UBaseStatusEffect* UStatusEffectPoolingSubsystem::GetStatusEffectInstance(UBaseStatusEffect* StatusEffect)
{
    UClass* WantedClass = StatusEffect->GetClass();

    FPooledStatusEffectsArray& Array = StatusEffects.FindOrAdd(WantedClass);

    FPooledStatusEffect* AvailableInstance = Array.StatusEffectsArray.FindByPredicate([](const FPooledStatusEffect& PooledEffect)
        {
            return !PooledEffect.bInUse;
        });

    if (!AvailableInstance)
    {
        FPooledStatusEffect NewPooledStatusEffect;
        UBaseStatusEffect* NewStatusEffectInstance = DuplicateObject<UBaseStatusEffect>(StatusEffect, this);
        NewPooledStatusEffect.StatusEffect = NewStatusEffectInstance;
        NewPooledStatusEffect.bInUse = true;
        Array.StatusEffectsArray.Add(NewPooledStatusEffect);
        return NewStatusEffectInstance;
    }
    else
    {
        AvailableInstance->StatusEffect->ReinitializeProperties(StatusEffect);
        AvailableInstance->bInUse = true;
        return AvailableInstance->StatusEffect;
    }
}

void UStatusEffectPoolingSubsystem::ReturnStatusEffectInstance(const UBaseStatusEffect* StatusEffect)
{
    UClass* Class = StatusEffect->GetClass();

    FPooledStatusEffectsArray* Array = StatusEffects.Find(Class);
    if (ensure(Array))
    {
        FPooledStatusEffect* Instance = Array->StatusEffectsArray.FindByPredicate([StatusEffect](const FPooledStatusEffect& PooledStatusEffect)
            {
                return StatusEffect == PooledStatusEffect.StatusEffect;
            });
        if (ensure(Instance))
            Instance->bInUse = false;
    }
}
