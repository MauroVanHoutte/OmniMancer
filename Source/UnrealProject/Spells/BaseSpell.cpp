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
	AActor::Destroy();
}

void ABaseSpell::AddStatusEffect(const FStatusEffect& effect)
{
	StatusEffects.Add(effect);
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


