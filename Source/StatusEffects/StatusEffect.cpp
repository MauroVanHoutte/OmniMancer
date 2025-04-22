// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffect.h"
#include <AIController.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/BaseCharacter.h"
#include "Health/AffiliationComponent.h"
#include "Health/BaseHealthComponent.h"
#include "Health/HealthManager.h"
#include "Health/HitHandlingComponent.h"
#include <Kismet/GameplayStatics.h>
#include <NiagaraComponent.h>
#include "Upgrades/StatUpgrades/StatComponent.h"
#include "SpellCasting/ElementManipulationComponent.h"
#include "StatusEffectHandlingComponent.h"
#include "Upgrades/StatUpgrades/StatBoosts.h"

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
	else if (bSeperateByInstigator)
	{
		UBaseStatusEffect** MatchingEffect = ActiveEffects.FindByPredicate([this](const UBaseStatusEffect* OtherEffect)
			{
				return Identifier == OtherEffect->Identifier
					&& Instigator == OtherEffect->Instigator;
			});
		if (MatchingEffect)
			return false;
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

FFormatNamedArguments UBaseStatusEffect::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add("TotalDuration", TotalDuration);
	return Args;
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
		UNiagaraComponent* Niagara = Target->GetComponentByClass<UNiagaraComponent>();
		if (IsValid(Niagara))
		{
			Niagara->SetVariableFloat(TEXT("Burning"), 1.f);
		}
	}

	return bNotRefreshed;
}

void UDamageOverTimeStatusEffect::Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	UNiagaraComponent* Niagara = Target->GetComponentByClass<UNiagaraComponent>();
	if (IsValid(Niagara))
	{
		UBaseStatusEffect** OtherBurnEffect = ActiveEffects.FindByPredicate([this](const UBaseStatusEffect* OtherEffect)
			{
				return OtherEffect->IsA<UDamageOverTimeStatusEffect>()
					&& this != OtherEffect;
			});
		if (!OtherBurnEffect)
		{
			Niagara->SetVariableFloat(TEXT("Burning"), 0.f);
		}
	}
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

FFormatNamedArguments UDamageOverTimeStatusEffect::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Append(Super::GetDescriptionArguments());
	Args.Add(TEXT("TickDamage"), Damage);
	Args.Add(TEXT("DamageInterval"), Interval);
	return Args;
}

bool UCurseStatusEffect::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	bool bNotRefreshed = Super::Apply(Target, ActiveEffects);

	if (bNotRefreshed)
	{
		UHealthManager* HealthManager = Target->GetComponentByClass<UHealthManager>();
		if (IsValid(HealthManager))
		{
			HealthManager->OnFatalDamageTakenDelegate.AddDynamic(this, &UCurseStatusEffect::OnFatalDamageTaken);
		}
	}

	return bNotRefreshed;
}

void UCurseStatusEffect::Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
 	FDamageEvent DamageEvent;
	Target->TakeDamage(Damage, DamageEvent, IsValid(Instigator) ? Instigator->Controller : nullptr, Instigator);

	UHealthManager* HealthManager = Target->GetComponentByClass<UHealthManager>();
	if (IsValid(HealthManager))
	{
		HealthManager->OnFatalDamageTakenDelegate.RemoveDynamic(this, &UCurseStatusEffect::OnFatalDamageTaken);
	}

	Super::Remove(Target, ActiveEffects);
}

void UCurseStatusEffect::OnFatalDamageTaken(UBaseHealthComponent* DamagedComponent, float DamageTaken, float OverkillDamage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(Instigator, DamagedComponent->GetOwner()->GetActorLocation(), SpreadRange, {}, APawn::StaticClass(), {Instigator, DamagedComponent->GetOwner() }, OverlappingActors);

	UAffiliationComponent* InstigatorAffiliation = Instigator->GetComponentByClass<UAffiliationComponent>();

	if (InstigatorAffiliation)
	{
		OverlappingActors = OverlappingActors.FilterByPredicate([InstigatorAffiliation](const AActor* CurrentActor)
			{
				UAffiliationComponent* CurrentActorAffiliation = CurrentActor->GetComponentByClass<UAffiliationComponent>();
				return !CurrentActorAffiliation || CurrentActorAffiliation->GetAffiliation() != InstigatorAffiliation->GetAffiliation();
			});
	}

	for (AActor* Actor : OverlappingActors)
	{
		UStatusEffectHandlingComponent* StatusHandling = Actor->GetComponentByClass<UStatusEffectHandlingComponent>();
		if (IsValid(StatusHandling))
		{
			StatusHandling->ApplyStatusEffect(this);
		}
	}
}

FFormatNamedArguments UCurseStatusEffect::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Append(Super::GetDescriptionArguments());
	Args.Add(TEXT("CurseDamage"), Damage);
	Args.Add(TEXT("SpreadRange"), SpreadRange);
	return Args;
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

FFormatNamedArguments UMovementSpeedStatusEffect::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Append(Super::GetDescriptionArguments());
	Args.Add(TEXT("SpeedReduction"), (1 - SpeedMultiplier) * 100);
	return Args;
}

bool UStunStatusEffect::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	bool bNotRefreshed = Super::Apply(Target, ActiveEffects);

	if (bNotRefreshed)
	{
		APawn* Pawn = Cast<APawn>(Target);
		if (IsValid(Pawn))
		{
			AAIController* Controller = Pawn->GetController<AAIController>();
			if (IsValid(Controller))
			{
				UBlackboardComponent* Blackboard = Controller->GetBlackboardComponent();
				Blackboard->SetValueAsBool(BlackboardKey, true);
			}
		}

		UNiagaraComponent* Niagara = Target->GetComponentByClass<UNiagaraComponent>();
		if (IsValid(Niagara))
		{
			Niagara->SetVariableFloat(TEXT("Stunned"), 1.f);
		}
	}

	return bNotRefreshed;
}

void UStunStatusEffect::Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	UBaseStatusEffect** OtherStunEffect = ActiveEffects.FindByPredicate([this](const UBaseStatusEffect* Effect)
		{ return Effect->IsA<UStunStatusEffect>() && Effect != this; });

	if (!OtherStunEffect || !IsValid(*OtherStunEffect))
	{
		APawn* Pawn = Cast<APawn>(Target);
		if (IsValid(Pawn))
		{
			AAIController* Controller = Pawn->GetController<AAIController>();
			if (IsValid(Controller))
			{
				UBlackboardComponent* Blackboard = Controller->GetBlackboardComponent();
				Blackboard->SetValueAsBool(BlackboardKey, false);
			}
		}

		UNiagaraComponent* Niagara = Target->GetComponentByClass<UNiagaraComponent>();
		if (IsValid(Niagara))
		{
			Niagara->SetVariableFloat(TEXT("Stunned"), 0.f);
		}
	}
}

FFormatNamedArguments UBaseStackingStatusEffect::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Append(Super::GetDescriptionArguments());
	Args.Add(TEXT("StackLimit"), StackLimit);
	return Args;
}

bool USpellMark::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	bool bNotRefreshed = Super::Apply(Target, ActiveEffects);
	
	if (bNotRefreshed)
	{
		UHitHandlingComponent* HitHandling = Target->GetComponentByClass<UHitHandlingComponent>();
		if (IsValid(HitHandling))
			HitHandling->OnHitRegisteredDelegate.AddDynamic(this, &USpellMark::OnHitTaken);
	}

	return bNotRefreshed;
}

void USpellMark::Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	UHitHandlingComponent* HitHandling = Target->GetComponentByClass<UHitHandlingComponent>();
	if (IsValid(HitHandling))
		HitHandling->OnHitRegisteredDelegate.RemoveDynamic(this, &USpellMark::OnHitTaken);
}

FFormatNamedArguments USpellMark::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Append(Super::GetDescriptionArguments());
	Args.Add(TEXT("TriggerSpell"), TriggerSpell->GetDisplayNameText());
	Args.Add(TEXT("MarkDamage"), Damage);
	return Args;
}

void USpellMark::OnHitTaken(AActor* HittingObject, AActor* HitActor)
{
	if (HittingObject->IsA(TriggerSpell))
	{
		FDamageEvent DamageEvent;
		HitActor->TakeDamage(Damage, DamageEvent, HittingObject->GetInstigatorController(), HittingObject);
		if (bRemoveOnTrigger)
		{
			RemainingDuration = 0;
		}
	}
}

bool UTempCooldownReduction::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	bool bNotRefreshed = Super::Apply(Target, ActiveEffects);

	if (bNotRefreshed)
	{
		if (!IsValid(SpellCasting))
		{
			SpellCasting = Target->GetComponentByClass<UElementManipulationComponent>();
		}

		if (IsValid(SpellCasting))
		{
			for (TSubclassOf<ABaseSpell> SpellType : ApplicableSpells)
			{
				SpellCasting->AddCooldownMultiplier(SpellType, CooldownMultiplier);
			}
		}
	}

	return bNotRefreshed;
}

void UTempCooldownReduction::Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	if (IsValid(SpellCasting))
	{
		for (TSubclassOf<ABaseSpell> SpellType : ApplicableSpells)
		{
			SpellCasting->AddCooldownMultiplier(SpellType, 1.f / CooldownMultiplier);
		}
	}
}

FFormatNamedArguments UTempCooldownReduction::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Append(Super::GetDescriptionArguments());
	for (size_t i = 0; i < ApplicableSpells.Num(); i++)
	{
		if (IsValid(ApplicableSpells[i]))
		{
			Args.Add(TEXT("ApplicableSpell") + i, ApplicableSpells[i]->GetDisplayNameText());
		}
	}
	Args.Add(TEXT("CooldownReduction"), (1 - CooldownMultiplier) * 100);
	return Args;
}

bool UExecuteEffect::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	bool bNotRefreshed = Super::Apply(Target, ActiveEffects);

	if (bNotRefreshed)
	{
		HealthManager = Target->GetComponentByClass<UHealthManager>();
		if (IsValid(HealthManager))
		{
			HealthManager->OnDamageTakenDelegate.AddDynamic(this, &UExecuteEffect::OnDamageTaken);
			HealthManager->TakeDamage(Target, 0.1f, nullptr, Instigator->GetController(), Instigator);
		}
	}

	return bNotRefreshed;
}

void UExecuteEffect::Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	if (IsValid(HealthManager))
	{
		HealthManager->OnDamageTakenDelegate.RemoveDynamic(this, &UExecuteEffect::OnDamageTaken);
	}
}

FFormatNamedArguments UExecuteEffect::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Append(Super::GetDescriptionArguments());
	Args.Add(TEXT("ExecutionThreshold"), ExecutionThreshold * 100);
	return Args;
}

void UExecuteEffect::OnDamageTaken(UBaseHealthComponent* DamagedComponent, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!DamagedComponent->IsDepleted() && HealthManager->GetLiveHealthComponentCount() == 1 && DamagedComponent->GetHealthPercentage() < ExecutionThreshold)
	{
		DamagedComponent->Kill(DamageType, Instigator ? Instigator->GetController() : nullptr, DamageCauser);
	}
}

TArray<TPair<UChainDamageEffect*, AActor*>> UChainDamageEffect::ActiveChainEffects{};
bool UChainDamageEffect::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	bool bNotRefreshed = Super::Apply(Target, ActiveEffects);

	if (bNotRefreshed)
	{
		ActiveChainEffects.Add(TPair<UChainDamageEffect*, AActor*>(this, Target));
		HealthManager = Target->GetComponentByClass<UHealthManager>();
		if (IsValid(HealthManager))
		{
			HealthManager->OnDamageTakenDelegate.AddDynamic(this, &UChainDamageEffect::OnDamageTaken);
		}
	}

	return bNotRefreshed;
}

void UChainDamageEffect::Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	ActiveChainEffects.RemoveAll([this, Target](const TPair<UChainDamageEffect*, AActor*>& Effect)
		{
			return Effect.Key == this && Effect.Value == Target;
		});

	if (IsValid(HealthManager))
	{
		HealthManager->OnDamageTakenDelegate.RemoveDynamic(this, &UChainDamageEffect::OnDamageTaken);
	}
}

FFormatNamedArguments UChainDamageEffect::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Append(Super::GetDescriptionArguments());
	Args.Add(TEXT("TransferPercentage"), TransferPercentage * 100);
	Args.Add(TEXT("ChainRange"), Range);
	return Args;
}

void UChainDamageEffect::OnDamageTaken(UBaseHealthComponent* DamagedComponent, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsValid(DamageType) || !DamageType->IsA(UChainDamageType::StaticClass()))
	{
		TArray<AActor*> ReachedActors;
		ReachedActors.Add(DamagedComponent->GetOwner());
		int NewlyReachedActors;
		do
		{
			NewlyReachedActors = 0;

			for (TPair<UChainDamageEffect*, AActor*> Pair : ActiveChainEffects)
			{
				if (IsValid(Pair.Key) && IsValid(Pair.Value) && !ReachedActors.Contains(Pair.Value))
				{
					for (size_t i = 0; i < ReachedActors.Num(); i++)
					{
						if (IsValid(ReachedActors[i]) && FVector::DistSquared(ReachedActors[i]->GetActorLocation(), Pair.Value->GetActorLocation()) < Range * Range)
						{
							ReachedActors.Add(Pair.Value);
							NewlyReachedActors++;
							break;
						}
					}
				}
			}
		} while (NewlyReachedActors > 0);

		for (AActor* ReachedActor : ReachedActors)
		{
			if (IsValid(ReachedActor) && ReachedActor != DamagedComponent->GetOwner())
			{
				FDamageEvent DamageEvent;
				DamageEvent.DamageTypeClass = UChainDamageType::StaticClass();
				ReachedActor->TakeDamage(Damage * TransferPercentage, DamageEvent, InstigatedBy, DamageCauser);
			}
		}
	}
}

bool UInvulnerabilityEffect::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	bool bNotRefreshed = Super::Apply(Target, ActiveEffects);
	if (bNotRefreshed)
	{
		if (IsValid(Target))
		{
			UHealthManager* HealthManager = Target->GetComponentByClass<UHealthManager>();
			if (IsValid(HealthManager))
			{
				HealthManager->SetInvulnerability(true);
			}
			UHitHandlingComponent* HitHandling = Target->GetComponentByClass<UHitHandlingComponent>();
			if (IsValid(HitHandling))
			{
				HitHandling->OnHitRegisteredDelegate.AddDynamic(this, &UInvulnerabilityEffect::OnHitRegistered);
			}
		}
	}
	return bNotRefreshed;
}

void UInvulnerabilityEffect::Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	if (IsValid(Target))
	{
		UHealthManager* HealthManager = Target->GetComponentByClass<UHealthManager>();
		if (IsValid(HealthManager))
		{
			HealthManager->SetInvulnerability(false);
		}
		UHitHandlingComponent* HitHandling = Target->GetComponentByClass<UHitHandlingComponent>();
		if (IsValid(HitHandling))
		{
			HitHandling->OnHitRegisteredDelegate.RemoveDynamic(this, &UInvulnerabilityEffect::OnHitRegistered);
		}
	}
}

void UInvulnerabilityEffect::OnHitRegistered(AActor* HittingObject, AActor* HitActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit registered while invulnerable"));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, HitActor->GetActorLocation());
}

bool UTemporaryStatBoostEffect::Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	bool bNotRefreshed = Super::Apply(Target, ActiveEffects);
	if (bNotRefreshed)
	{
		StatBoost->Apply(Target);
	}
	return bNotRefreshed;
}

void UTemporaryStatBoostEffect::Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects)
{
	StatBoost->Remove(Target);
}

FFormatNamedArguments UTemporaryStatBoostEffect::GetDescriptionArguments()
{
	return StatBoost->GetDescriptionArguments();
}
