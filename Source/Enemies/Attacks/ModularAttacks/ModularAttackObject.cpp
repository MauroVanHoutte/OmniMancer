// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Attacks/ModularAttacks/ModularAttackObject.h"
#include "Enemies/Attacks/ModularAttacks/ModularAttackEffect/ModularAttackEffect.h"
#include "Enemies/Attacks/ModularAttacks/ModularAttackTargetting/ModularAttackTargetting.h"
#include "Enemies/Attacks/ModularAttacks/ModularAttackRequirements/ModularAttackRequirements.h"

void UModularAttackObject::TickAttack(float DeltaTime)
{
	if (IsValid(AttackEffect))
		AttackEffect->TickAttack(DeltaTime);
}

void UModularAttackObject::OnBeginPlay(AActor* Owner)
{
	if (IsValid(AttackRequirement))
		AttackRequirement->OnBeginPlay(Owner);
	if (IsValid(AttackEffect))
	{
		AttackEffect->OnBeginPlay(Owner);
		AttackEffect->OnAttackEffectHitDelegate.AddDynamic(this, &UModularAttackObject::OnEffectHit);
		AttackEffect->OnAttackCompletedDelegate.AddDynamic(this, &UModularAttackObject::OnEffectCompleted);
	}
}

void UModularAttackObject::OnEndPlay()
{
	if (IsValid(AttackRequirement))
	{
		AttackRequirement->OnEndPlay();
		AttackRequirement->ConditionalBeginDestroy();
	}

	if (IsValid(AttackEffect))
	{
		AttackEffect->OnEndPlay();
		AttackEffect->ConditionalBeginDestroy();
	}
}

bool UModularAttackObject::AreAttackRequirementsMet(AActor* Target)
{
	return IsValid(AttackRequirement) ? AttackRequirement->AreRequirementsMet(Target) : true;
}

void UModularAttackObject::InitiateAttack(AActor* Target, const FVector& Location)
{
	if (IsValid(AttackEffect))
	{
		AttackEffect->ExecuteEffect(Target, Location);
	}
}

bool UModularAttackObject::WasActorHitBefore(AActor* TriggeredActor, UPrimitiveComponent* ColliderComponent)
{
	return IsValid(AttackEffect) ? AttackEffect->WasActorHitBefore(TriggeredActor, ColliderComponent) : true;
}

void UModularAttackObject::OnHitTriggered(AActor* HitActor, UPrimitiveComponent* ColliderComponent)
{
	if (IsValid(AttackEffect)) AttackEffect->OnHitTriggered(HitActor, ColliderComponent);
}

void UModularAttackObject::InterruptAttack()
{
	Super::InterruptAttack();

	if (IsValid(AttackEffect))
	{
		AttackEffect->InterruptEffect();
	}
}

void UModularAttackObject::OnEffectHit(UModularAttackEffectBase* Effect, AActor* AttackActor, AActor* HitActor)
{
	OnAttackHitDelegate.Broadcast(this, AttackActor, HitActor);
}

void UModularAttackObject::OnEffectCompleted(class UModularAttackEffectBase* Effect)
{
	OnAttackCompletedDelegate.Broadcast(this);
}
