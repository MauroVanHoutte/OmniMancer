// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerHandlingComponent.h"
#include "Health/HealthManager.h"
#include "SpellCasting/ElementManipulationComponent.h"

// Sets default values for this component's properties
UTriggerHandlingComponent::UTriggerHandlingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTriggerHandlingComponent::Initialize(UElementManipulationComponent* ElementComponent, UHealthManager* HealthManager)
{
	ElementComponent->OnSpellCastedDelegate.AddDynamic(this, &UTriggerHandlingComponent::TriggerSpellCasted);
	ElementComponent->OnBasicAttackCastedDelegate.AddDynamic(this, &UTriggerHandlingComponent::TriggerBasicAttackCasted);
	ElementComponent->OnSpellHitDelegate.AddDynamic(this, &UTriggerHandlingComponent::TriggerSpellHit);
	ElementComponent->OnBasicAttackHitDelegate.AddDynamic(this, &UTriggerHandlingComponent::TriggerBasicAttackHit);
	HealthManager->OnDamageTakenDelegate.AddDynamic(this, &UTriggerHandlingComponent::TriggerDamageTaken);

	TriggerEffects.FindOrAdd(TriggerCondition::OnBasicAttackCast);
	TriggerEffects.FindOrAdd(TriggerCondition::OnBasicAttackHit);
	TriggerEffects.FindOrAdd(TriggerCondition::OnSpellCast);
	TriggerEffects.FindOrAdd(TriggerCondition::OnSpellHit);
	TriggerEffects.FindOrAdd(TriggerCondition::OnTakeHit);
	TriggerEffects.FindOrAdd(TriggerCondition::OnDamageDealt);
	TriggerEffects.FindOrAdd(TriggerCondition::OnEnemyKiled);
}

void UTriggerHandlingComponent::AddTriggerEffect(TriggerCondition Condition, UBaseTriggerEffect* TriggerEffect)
{
	TriggerEffect->Instigator = GetOwner<APawn>();
	TriggerEffects.FindOrAdd(Condition).TriggerArray.Add(TriggerEffect);
}

void UTriggerHandlingComponent::TriggerDamageDealt(AActor* DamageCause, AActor* DamagedActor, float Damage)
{
	ABaseSpell* Spell = Cast<ABaseSpell>(DamageCause);
	TriggerTriggerEffects(TriggerCondition::OnDamageDealt, Spell, DamagedActor, Damage);
}

void UTriggerHandlingComponent::TriggerEnemyKilled(AActor* DamageCause, AActor* KilledEnemy, float Damage)
{
	ABaseSpell* Spell = Cast<ABaseSpell>(DamageCause);
	TriggerTriggerEffects(TriggerCondition::OnEnemyKiled, Spell, KilledEnemy, Damage);
}

void UTriggerHandlingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerHandlingComponent::TriggerSpellCasted(AActor* Caster, ABaseSpell* Spell)
{
	if (ensure(IsValid(Spell)))
	{
		TriggerTriggerEffects(TriggerCondition::OnSpellCast, Spell, nullptr, Spell->GetDamage());
	}
}

void UTriggerHandlingComponent::TriggerBasicAttackCasted(AActor* Caster, ABaseSpell* Spell)
{
	if (ensure(IsValid(Spell)))
	{
		TriggerTriggerEffects(TriggerCondition::OnBasicAttackCast, Spell, nullptr, Spell->GetDamage());
	}
}

void UTriggerHandlingComponent::TriggerSpellHit(ABaseSpell* Spell, AActor* HitActor)
{
	if (ensure(IsValid(Spell)))
	{
		TriggerTriggerEffects(TriggerCondition::OnSpellHit, Spell, HitActor, Spell->GetDamage());
	}
}

void UTriggerHandlingComponent::TriggerBasicAttackHit(ABaseSpell* Spell, AActor* HitActor)
{
	if (ensure(IsValid(Spell)))
	{
		TriggerTriggerEffects(TriggerCondition::OnBasicAttackHit, Spell, HitActor, Spell->GetDamage());
	}
}

void UTriggerHandlingComponent::TriggerDamageTaken(UBaseHealthComponent* DamagedComponent, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	ABaseSpell* SourceSpell = Cast<ABaseSpell>(DamageCauser);
	TriggerTriggerEffects(TriggerCondition::OnTakeHit, SourceSpell, InstigatedBy ? InstigatedBy->GetPawn() : nullptr, Damage);
}

void UTriggerHandlingComponent::TriggerTriggerEffects(TriggerCondition Condition, ABaseSpell* Spell, AActor* Target, float Damage)
{
	FTriggerArray* TriggerEffectsForCondition = TriggerEffects.Find(Condition);

	if (ensure(TriggerEffectsForCondition))
	{
		for (UBaseTriggerEffect* TriggerEffect : TriggerEffectsForCondition->TriggerArray)
		{
			TriggerEffect->Trigger(GetOwner(), Spell, Target, Damage);
		}
	}
}