// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularExtraConditions.h"
#include "SpellCasting/Spells/BaseSpell.h"
#include "SpellCasting/ElementManipulationComponent.h"
#include "StatusEffects/StatusEffectHandlingComponent.h"
#include "UObject/Class.h"

bool UCompoundModularCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	switch (Operation)
	{
	case CompoundConditionOperation::AND:
		for (UModularExtraConditionsBase* Condition : Conditions)
		{
			if (!IsValid(Condition) || !Condition->CheckCondition(triggerOwner, spell, target))
			{
				return false;
			}
		}
		return true;
		break;
	case CompoundConditionOperation::OR:
		for (UModularExtraConditionsBase* Condition : Conditions)
		{
			if (IsValid(Condition) && Condition->CheckCondition(triggerOwner, spell, target))
			{
				return true;
			}
		}
		break;
	default:
		break;
	}
	return false;
}

void UCompoundModularCondition::OnExecution(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	for (UModularExtraConditionsBase* Condition : Conditions)
	{
		Condition->OnExecution(targetLocations, targetActors, Damage, instigator);
	}
}

FFormatNamedArguments UCompoundModularCondition::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	for (UModularExtraConditionsBase* Condition : Conditions)
	{
		Args.Append(Condition->GetDescriptionArguments());
	}
	return Args;
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

FFormatNamedArguments USpellCondition::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	for (size_t i = 0; i < AllowedSpellTypes.Num(); i++)
	{
		Args.Add(TEXT("AllowedSpell") + FString::FromInt(i), AllowedSpellTypes[i]->GetDisplayNameText());
	}
	return Args;
}

bool UCooldownCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	return !triggerOwner->GetWorld()->GetTimerManager().IsTimerActive(TimerHandle);
}

void UCooldownCondition::OnExecution(const TArray<FVector>& targetLocations, const TArray<class AActor*>& targetActors, float Damage, APawn* instigator)
{
	instigator->GetWorld()->GetTimerManager().SetTimer(TimerHandle, CooldownTime, false);
}

FFormatNamedArguments UCooldownCondition::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("CooldownTime"), CooldownTime);
	return Args;
}

bool USpellStatusEffectCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	return spell->GetStatusEffectsRef().FindByPredicate([this](const UBaseStatusEffect* StatusEffect) {return StatusEffect->IsA(RequiredStatusEffect); }) != nullptr;
}

FFormatNamedArguments USpellStatusEffectCondition::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("RequiredSpellStatusEffect"), RequiredStatusEffect->GetDisplayNameText());
	return Args;
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

FFormatNamedArguments UTargetAffectedByStatusEffectCondition::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("RequiredAffectingStatusEffect"), RequiredStatusEffectType->GetDisplayNameText());
	return Args;
}

bool UTargetInRangeCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	if (!IsValid(triggerOwner) || !IsValid(target))
		return false;

	return FVector::DistSquared(triggerOwner->GetActorLocation(), target->GetActorLocation()) < Range * Range;
}

FFormatNamedArguments UTargetInRangeCondition::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("MaximumDistance"), Range);
	return Args;
}

bool URandomChanceCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	return FMath::FRand() <= (Chance/100.f);
}

FFormatNamedArguments URandomChanceCondition::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("ActivationChance"), Chance);
	return Args;
}

bool UNthActivationCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	ActivatonCount++;
	return ActivatonCount % ActivationInterval == 0;
}

FFormatNamedArguments UNthActivationCondition::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("ActivationInterval"), ActivationInterval);
	return Args;
}

bool USpellElementCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	UElementManipulationComponent* SpellCasting = triggerOwner->GetComponentByClass<UElementManipulationComponent>();
	if (IsValid(SpellCasting))
	{
		const TArray<FSpellConfig>& Configs = SpellCasting->GetSpellConfigs();
		const FSpellConfig* Config = Configs.FindByPredicate([spell](const FSpellConfig& Config)
		{
			return Config.Spell->IsChildOf(spell->GetClass());
		});

		if (Config && Config->ElementCombination.Contains(Element))
		{
			return true;
		}
	}
	return false;
}

FFormatNamedArguments USpellElementCondition::GetDescriptionArguments()
{
	const UEnum* WizardElementEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("WizardElement"));
	FFormatNamedArguments Args;
	Args.Add(TEXT("SpellElement"), WizardElementEnum->GetDisplayNameTextByValue((int64)Element));
	return Args;
}

bool ULatestElementCondition::CheckCondition(AActor* triggerOwner, ABaseSpell* spell, AActor* target)
{
	UElementManipulationComponent* SpellCastingComp = triggerOwner->GetComponentByClass<UElementManipulationComponent>();
	if (IsValid(SpellCastingComp))
	{
		const TArray<WizardElement>& ActiveElements = SpellCastingComp->GetActiveElements();
		if (!ActiveElements.IsEmpty() && ActiveElements[0] == Element)
			return true;
	}
	return false;
}

FFormatNamedArguments ULatestElementCondition::GetDescriptionArguments()
{
	const UEnum* WizardElementEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("WizardElement"));
	FFormatNamedArguments Args;
	Args.Add(TEXT("LatestElement"), WizardElementEnum->GetDisplayNameTextByValue((int64)Element));
	return Args;
}
