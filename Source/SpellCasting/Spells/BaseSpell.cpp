// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpell.h"
#include "WizardCharacter.h"
#include "StatusEffects/StatusEffectHandlingComponent.h"

// Sets default values
ABaseSpell::ABaseSpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(false);
}

void ABaseSpell::Destroy()
{
	OnDeath();
	AActor::Destroy();
}

void ABaseSpell::OnDeath()
{
}

//void ABaseSpell::AddStatusEffect(const FStatusEffect& effect)
//{
//	StatusEffects.Add(effect);
//}
//
//void ABaseSpell::SetBurnParams(bool applyBurns, float tickDamage, float tickInterval, float duration)
//{
//	auto effect = StatusEffects.FindByPredicate([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Damage; });
//
//	if (applyBurns)
//	{
//		if (effect != nullptr)
//		{
//			effect->Value = tickDamage;
//			effect->Interval = tickInterval;
//			effect->Duration = duration * BurnDurationMultiplier;
//		}
//		else
//		{
//			StatusEffects.Add(FStatusEffect(Type::Damage, tickInterval, tickDamage, duration * BurnDurationMultiplier, this));
//		}
//	}
//	else
//	{
//		if (effect != nullptr)
//		{
//			StatusEffects.RemoveAll([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Damage; });
//		}
//	}
//}
//
//void ABaseSpell::SetSlowParams(bool applySlow, float value, float duration)
//{
//	auto effect = StatusEffects.FindByPredicate([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Slow; });
//
//	if (applySlow)
//	{
//		if (effect != nullptr)
//		{
//			effect->Value = value;
//			effect->Duration = duration * SlowDurationMultiplier;
//		}
//		else
//		{
//			StatusEffects.Add(FStatusEffect(Type::Slow, -1, value, duration * SlowDurationMultiplier, this));
//		}
//	}
//	else
//	{
//		if (effect != nullptr)
//		{
//			StatusEffects.RemoveAll([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Slow; });
//		}
//	}
//}
//
//void ABaseSpell::SetStunParams(bool applyStun, float duration)
//{
//	auto effect = StatusEffects.FindByPredicate([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Stun; });
//
//	if (applyStun)
//	{
//		if (effect != nullptr)
//		{
//			effect->Duration = duration * StunDurationMultiplier;
//		}
//		else
//		{
//			StatusEffects.Add(FStatusEffect(Type::Stun, -1, -1, duration * StunDurationMultiplier, this));
//		}
//	}
//	else
//	{
//		if (effect != nullptr)
//		{
//			StatusEffects.RemoveAll([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Stun; });
//		}
//	}
//
//}
//
//void ABaseSpell::SetCurseParams(bool applyCurse, float damage, float range, float duration)
//{
//	auto effect = StatusEffects.FindByPredicate([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Curse; });
//
//	if (applyCurse)
//	{
//		if (effect != nullptr)
//		{
//			effect->Duration = duration;
//			effect->Value = damage;
//			effect->Interval = range;
//		}
//		else
//		{
//			StatusEffects.Add(FStatusEffect(Type::Curse, range, damage, duration, this));
//		}
//	}
//	else
//	{
//		if (effect != nullptr)
//		{
//			StatusEffects.RemoveAll([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Curse; });
//		}
//	}
//}

void ABaseSpell::InitSpell(const FVector& targetLocation, APawn* caster)
{
	SetOwner(caster);
	SetInstigator(caster);

	for (UBaseStatusEffect* StatusEffect : StatusEffects)
	{
		StatusEffect->Instigator = caster;
	}

	SetActorEnableCollision(true);

	/*FireLevel = wizard->GetCurrentElementLevel(WizardElement::Fire);
	FrostLevel = wizard->GetCurrentElementLevel(WizardElement::Frost);
	WindLevel = wizard->GetCurrentElementLevel(WizardElement::Wind);
	SlowDurationMultiplier = wizard->GetSlowDurationMultiplier();
	StunDurationMultiplier = wizard->GetStunDurationMultiplier();
	BurnDurationMultiplier = wizard->GetBurnDurationMultiplier();
	DamageMultiplier = wizard->GetSpellDamageMultiplier();*/
}

void ABaseSpell::SetBaseDamage(float damage)
{
	Damage = damage;
}

float ABaseSpell::GetBaseDamage()
{
	return Damage;
}

void ABaseSpell::SetDamageMultiplier(float damageMultiplier)
{
	DamageMultiplier = damageMultiplier;
}

float ABaseSpell::GetDamageMultiplier()
{
	return DamageMultiplier;
}

// Called when the game starts or when spawned
void ABaseSpell::BeginPlay()
{
	Super::BeginPlay();
	SetLifeTime(LifeSpan);
}

void ABaseSpell::SetLifeTime(float lifespan)
{
	if (lifespan < 0)
		return;

	GetWorld()->GetTimerManager().SetTimer(LifeTimer, this, &ABaseSpell::Destroy, lifespan);
}

// Called every frame
void ABaseSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABaseSpell::GetFinalDamage() const
{
	return Damage*DamageMultiplier;
}

TArray<UBaseStatusEffect*>& ABaseSpell::GetStatusEffectsRef()
{
	return StatusEffects;
}

TArray<AActor*>& ABaseSpell::GetHitActorsRef()
{
	return HitActors;
}

//const TArray<FStatusEffect>& ABaseSpell::GetStatusEffects() const
//{
//	return StatusEffects;
//}

void ABaseSpell::AddHitActor( AActor* actor)
{
	HitActors.Add(actor);
}

bool ABaseSpell::WasActorHit(AActor* actor) const
{
	int32 idx;
	return HitActors.Find(actor, idx);
}

void ABaseSpell::OnHit(AActor* hitActor)
{
	UStatusEffectHandlingComponent* StatusHandlingComponent = hitActor->GetComponentByClass<UStatusEffectHandlingComponent>();

	if (IsValid(StatusHandlingComponent))
	{
		for (UBaseStatusEffect* StatusEffect : StatusEffects)
		{
			StatusHandlingComponent->ApplyStatusEffect(StatusEffect);
		}
	}

	OnSpellHitDelegate.Broadcast(this, hitActor);
	//auto caster = Cast<AWizardCharacter>(GetOwner());
	//if (ApplyWizardOnHitEffects && caster != nullptr && caster->IsValidLowLevel())
	//{
	//	caster->OnSpellHitEnemy(this, hitActor); //activate on hit triggers
	//}
}


