// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpell.h"

// Sets default values
ABaseSpell::ABaseSpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseSpell::Destroy()
{
	OnDeath();
	AActor::Destroy();
}

void ABaseSpell::OnDeath()
{
}

void ABaseSpell::AddStatusEffect(const FStatusEffect& effect)
{
	StatusEffects.Add(effect);
}

void ABaseSpell::SetBurnParams(bool applyBurns, float tickDamage, float tickInterval, float duration)
{
	auto effect = StatusEffects.FindByPredicate([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Damage; });

	if (applyBurns)
	{
		if (effect != nullptr)
		{
			effect->Value = tickDamage;
			effect->Interval = tickInterval;
			effect->Duration = duration;
		}
		else
		{
			StatusEffects.Add(FStatusEffect(Type::Damage, tickInterval, tickDamage, duration, this));
		}
	}
	else
	{
		if (effect != nullptr)
		{
			StatusEffects.RemoveAll([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Damage; });
		}
	}
}

void ABaseSpell::SetSlowParams(bool applySlow, float value, float duration)
{
	auto effect = StatusEffects.FindByPredicate([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Slow; });

	if (applySlow)
	{
		if (effect != nullptr)
		{
			effect->Value = value;
			effect->Duration = duration;
		}
		else
		{
			StatusEffects.Add(FStatusEffect(Type::Slow, -1, value, duration, this));
		}
	}
	else
	{
		if (effect != nullptr)
		{
			StatusEffects.RemoveAll([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Slow; });
		}
	}
}

void ABaseSpell::SetStunParams(bool applyStun, float duration)
{
	auto effect = StatusEffects.FindByPredicate([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Stun; });

	if (applyStun)
	{
		if (effect != nullptr)
		{
			effect->Duration = duration;
		}
		else
		{
			StatusEffects.Add(FStatusEffect(Type::Stun, -1, -1, duration, this));
		}
	}
	else
	{
		if (effect != nullptr)
		{
			StatusEffects.RemoveAll([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Stun; });
		}
	}

}

void ABaseSpell::SetCurseParams(bool applyCurse, float damage, float range, float duration)
{
	auto effect = StatusEffects.FindByPredicate([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Curse; });

	if (applyCurse)
	{
		if (effect != nullptr)
		{
			effect->Duration = duration;
			effect->Value = damage;
			effect->Interval = range;
		}
		else
		{
			StatusEffects.Add(FStatusEffect(Type::Curse, range, damage, duration, this));
		}
	}
	else
	{
		if (effect != nullptr)
		{
			StatusEffects.RemoveAll([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Curse; });
		}
	}
}

void ABaseSpell::InitSpell(const FVector& casterLocation, const FVector& targetLocation, const FVector& projectileDirection, AActor* owner, APawn* instigator, int fireLevel, int frostLevel, int windLevel)
{
	SetOwner(owner);
	SetInstigator(instigator);
	FireLevel = fireLevel;
	FrostLevel = frostLevel;
	WindLevel = windLevel;
}

void ABaseSpell::SetDamage(float damage)
{
	Damage = damage;
}

// Called when the game starts or when spawned
void ABaseSpell::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(LifeTimer, this, &ABaseSpell::Destroy, LifeSpan);
}

void ABaseSpell::SetLifeTime(float lifespan)
{
	GetWorld()->GetTimerManager().SetTimer(LifeTimer, this, &ABaseSpell::Destroy, lifespan);
}

// Called every frame
void ABaseSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABaseSpell::GetDamage() const
{
	return Damage*(1+(DamageMultiplier/100.f));
}

const TArray<FStatusEffect>& ABaseSpell::GetStatusEffects() const
{
	return StatusEffects;
}

void ABaseSpell::SetDamageMultiplier(int damageMultiplier)
{
	DamageMultiplier = damageMultiplier;
}

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
}


