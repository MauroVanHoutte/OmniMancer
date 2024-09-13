// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularExtraConditions.h"
#include "SpellCasting/Spells/BaseSpell.h"
#include "StatusEffects/StatusEffectHandlingComponent.h"

bool UCompoundModularCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	bool CheckOne = ConditionOne->CheckCondition(triggerOwner, spell, target);
	bool CheckTwo = ConditionTwo->CheckCondition(triggerOwner, spell, target);

	switch (Operation)
	{
	case CompoundConditionOperation::AND:
		return CheckOne && CheckTwo;
		break;
	case CompoundConditionOperation::OR:
		return CheckOne || CheckTwo;
		break;
	default:
		break;
	}
	return false;
}

void UCompoundModularCondition::OnExecution(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	ConditionOne->OnExecution(targetLocations, targetActors, Damage, instigator);
	ConditionTwo->OnExecution(targetLocations, targetActors, Damage, instigator);
}

bool USpellCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	for (const TSubclassOf<ABaseSpell>& SpellType : AllowedSpellTypes)
	{
		if (spell->IsA(SpellType))
			return true;
	}

	return false;
}

bool UCooldownCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	return !triggerOwner->GetWorld()->GetTimerManager().IsTimerActive(TimerHandle);
}

void UCooldownCondition::OnExecution(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	instigator->GetWorld()->GetTimerManager().SetTimer(TimerHandle, CooldownTime, false);
}

bool USpellStatusEffectCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	return spell->GetStatusEffectsRef().FindByPredicate([this](const UBaseStatusEffect* StatusEffect) {return StatusEffect->IsA(RequiredStatusEffect); }) != nullptr;
}

bool UTargetAffectedByStatusEffectCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	UStatusEffectHandlingComponent* StatusEffectHandling = target->GetComponentByClass<UStatusEffectHandlingComponent>();

	if (!StatusEffectHandling)
		return false;

	const TArray<UBaseStatusEffect*>& ActiveEffects = StatusEffectHandling->GetActiveStatusEffects();
	UBaseStatusEffect* const* FoundEffect = ActiveEffects.FindByPredicate([this](const UBaseStatusEffect* Effect)
		{
			return Effect->IsA(RequiredStatusEffectType);
		});

	return FoundEffect != nullptr;
}

bool UTargetInRangeCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	if (!IsValid(triggerOwner) || !IsValid(target))
		return false;

	return FVector::DistSquared(triggerOwner->GetActorLocation(), target->GetActorLocation()) < Range * Range;
}
