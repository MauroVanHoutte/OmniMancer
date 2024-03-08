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
}

void UTriggerHandlingComponent::AddTriggerEffect(TriggerCondition Condition, UBaseTriggerEffect* TriggerEffect)
{
	TriggerEffects.FindOrAdd(Condition).TriggerArray.Add(TriggerEffect);
}

void UTriggerHandlingComponent::BeginPlay()
{
}

void UTriggerHandlingComponent::TriggerSpellCasted(AActor* Caster, ABaseSpell* Spell)
{
	TriggerTriggerEffects(TriggerCondition::OnSpellCast, Spell, nullptr);
}

void UTriggerHandlingComponent::TriggerBasicAttackCasted(AActor* Caster, ABaseSpell* Spell)
{
	TriggerTriggerEffects(TriggerCondition::OnBasicAttackCast, Spell, nullptr);
}

void UTriggerHandlingComponent::TriggerSpellHit(ABaseSpell* Spell, AActor* HitActor)
{
	TriggerTriggerEffects(TriggerCondition::OnSpellHit, Spell, HitActor);
}

void UTriggerHandlingComponent::TriggerBasicAttackHit(ABaseSpell* Spell, AActor* HitActor)
{
	TriggerTriggerEffects(TriggerCondition::OnBasicAttackHit, Spell, HitActor);
}

void UTriggerHandlingComponent::TriggerDamageTaken(UBaseHealthComponent* DamagedComponent, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	ABaseSpell* SourceSpell = Cast<ABaseSpell>(DamageCauser);
	TriggerTriggerEffects(TriggerCondition::OnTakeHit, SourceSpell, InstigatedBy->GetPawn());
}

void UTriggerHandlingComponent::TriggerTriggerEffects(TriggerCondition Condition, ABaseSpell* Spell, AActor* Target)
{
	FTriggerArray* TriggerEffectsForCondition = TriggerEffects.Find(Condition);

	if (ensure(TriggerEffectsForCondition))
	{
		for (UBaseTriggerEffect* TriggerEffect : TriggerEffectsForCondition->TriggerArray)
		{
			TriggerEffect->Trigger(GetOwner(), Spell, Target);
		}
	}
}
