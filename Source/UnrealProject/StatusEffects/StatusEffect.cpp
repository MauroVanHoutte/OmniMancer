// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffect.h"
#include "Health\AffiliationComponent.h"
#include "Upgrades\StatUpgrades\StatComponent.h"

//FStatusEffect::FStatusEffect(Type type, float interval, float value, float duration, UObject* cause)
//	: EffectType{type}
//	, Interval{interval}
//	, Value{value}
//	, Duration{duration}
//	, Cause{cause}
//{
//}
//
//FStatusEffect::~FStatusEffect()
//{
//}

bool UBaseStatusEffect::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	if (bRefreshDuration)
	{
		UBaseStatusEffect** MatchingEffect = ActiveEffects.FindByPredicate([this](const UBaseStatusEffect* OtherEffect)
			{
				return Identifier == OtherEffect->Identifier
					&& (!bSeperateByInstigator || Instigator == OtherEffect->Instigator);
			});

		if (MatchingEffect && *MatchingEffect)
		{
			(*MatchingEffect)->RefreshDuration();
			return false;
		}
	}

	RemainingDuration = TotalDuration; 
	return true;
}

void UBaseStatusEffect::Update(float DeltaTime, AActor* Target)
{
	RemainingDuration -= DeltaTime;
}

bool UBaseStatusEffect::IsExpired(AActor* Target)
{
	return RemainingDuration <= 0;
}

void UBaseStatusEffect::SetParamsFrom(UBaseStatusEffect* StatusEffect)
{
	ensure(StaticClass() == StatusEffect->StaticClass());
	UEngine::CopyPropertiesForUnrelatedObjects(StatusEffect, this);
}

bool UBaseStackingStatusEffect::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	UBaseStackingStatusEffect** MatchingEffect = (UBaseStackingStatusEffect**)ActiveEffects.FindByPredicate([this](const UBaseStatusEffect* OtherEffect)
		{
			return Identifier == OtherEffect->Identifier
				&& !bSeperateByInstigator || Instigator == OtherEffect->Instigator;
		});

	if (MatchingEffect && IsValid(*MatchingEffect))
	{
		(*MatchingEffect)->AddStack();
		if (bRefreshDuration)
		{
			if (MatchingEffect && *MatchingEffect)
			{
				(*MatchingEffect)->RefreshDuration();
			}
		}

		return false;
	}

	RemainingDuration = TotalDuration;
	return true;
}

bool UDamageOverTimeStatusEffect::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	bool bNotRefreshed = Super::Apply(Target, ActiveEffects);
	
	if (bNotRefreshed)
	{
		FDamageEvent DamageEvent;
		Target->TakeDamage(Damage, DamageEvent, IsValid(Instigator) ? Instigator->Controller : nullptr, Instigator);
	}

	return bNotRefreshed;
}

void UDamageOverTimeStatusEffect::Update(float DeltaTime, AActor* Target)
{
	Super::Update(DeltaTime, Target);
	Timer += DeltaTime;
	if (Timer > Interval)
	{
		Timer -= Interval;
		FDamageEvent DamageEvent;
		Target->TakeDamage(Damage, DamageEvent, IsValid(Instigator) ? Instigator->Controller : nullptr, Instigator);
	}
}

bool UCurseStatusEffect::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	UBaseStatusEffect** MatchingEffect = ActiveEffects.FindByPredicate([this](const UBaseStatusEffect* OtherEffect)
		{
			return Identifier == OtherEffect->Identifier
				&& (!bSeperateByInstigator || Instigator == OtherEffect->Instigator);
		});

	return !MatchingEffect;
}

void UCurseStatusEffect::Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	FDamageEvent DamageEvent;
	Target->TakeDamage(Damage, DamageEvent, IsValid(Instigator) ? Instigator->Controller : nullptr, Instigator);
	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(Target->GetWorld(), Target->GetActorLocation(), SpreadRange, {}, APawn::StaticClass(), { Instigator, Target }, OverlappingActors);

	UAffiliationComponent* InstigatorAffiliation = Instigator->GetComponentByClass<UAffiliationComponent>();

	if (InstigatorAffiliation)
	{
		OverlappingActors = OverlappingActors.FilterByPredicate([InstigatorAffiliation](const AActor* CurrentActor)
			{
				UAffiliationComponent* CurrentActorAffiliation = CurrentActor->GetComponentByClass<UAffiliationComponent>();
				return !CurrentActorAffiliation || CurrentActorAffiliation->GetAffiliation() != InstigatorAffiliation->GetAffiliation();
			});
	}

	Super::Remove(Target, ActiveEffects);
}

bool UMovementSpeedStatusEffect::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	bool bNotRefreshed = Super::Apply(Target, ActiveEffects);

	if (bNotRefreshed)
	{
		UStatComponent* Stats = Target->GetComponentByClass<UStatComponent>();
		if (IsValid(Stats))
		{
			Stats->SetSpeedMultiplier(Stats->GetSpeedMultiplier() * SpeedMultiplier);
		}
	}

	return bNotRefreshed;
}

void UMovementSpeedStatusEffect::Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	UStatComponent* Stats = Target->GetComponentByClass<UStatComponent>();
	if (IsValid(Stats))
	{
		Stats->SetSpeedMultiplier(Stats->GetSpeedMultiplier() / SpeedMultiplier);
	}
}
