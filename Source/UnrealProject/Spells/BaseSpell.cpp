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

// Called when the game starts or when spawned
void ABaseSpell::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(m_LifeTimer, this, &ABaseSpell::Destroy, m_LifeSpan);
}

// Called every frame
void ABaseSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABaseSpell::GetDamage() const
{
	return m_Damage*(1+(m_DamageMultiplier/100.f));
}

const TArray<FStatusEffect>& ABaseSpell::GetStatusEffects() const
{
	return m_StatusEffects;
}

void ABaseSpell::SetDamageMultiplier(int damageMultiplier)
{
	m_DamageMultiplier = damageMultiplier;
}

void ABaseSpell::AddHitActor( AActor* actor)
{
	m_HitActors.Add(actor);
}

bool ABaseSpell::WasActorHit(AActor* actor) const
{
	int32 idx;
	return m_HitActors.Find(actor, idx);
}

void ABaseSpell::OnHit(AActor* hitActor)
{
}


