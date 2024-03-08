// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularTriggerEffects.h"
#include "UnrealProject/StatusEffects/StatusEffectHandlingComponent.h"
#include "StatusEffects/StatusEffect.h"

void UStatusEffectTriggerEffect::ExecuteEffect(const TArray<FVector>& targetLocations, const TArray<AActor*>& targetActors, class APawn* instigator)
{
	StatusEffect->Instigator = instigator;
	for (AActor* Actor : targetActors)
	{
		UStatusEffectHandlingComponent* StatusEffectHandlingComp = Actor->GetComponentByClass<UStatusEffectHandlingComponent>();
		if (IsValid(StatusEffectHandlingComp))
		{
			StatusEffectHandlingComp->ApplyStatusEffect(StatusEffect);
		}
	}
}
