// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/Attacks/SpellAttackObject.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include "SpellCasting/Spells/BaseSpell.h"
#include <AIController.h>

void USpellAttackObject::OnEndPlay()
{
	if (IsValid(GetWorld()))
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void USpellAttackObject::InitiateAttack(AActor* Target, const FVector& Location)
{
	Super::InitiateAttack(Target, Location);

	FTimerManager& manager = GetWorld()->GetTimerManager();

	if (bUseLocationTarget)
	{
		Cast<APawn>(OwningActor)->GetController<AAIController>()->SetFocus(nullptr);
	}
	else
	{
		Cast<APawn>(OwningActor)->GetController<AAIController>()->SetFocus(Target);
	}

	if (!manager.IsTimerActive(TimerHandle))
	{
		if (ChargeTime <= 0)
		{
			CastSpell();
		}
		else
		{
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &USpellAttackObject::CastSpell);
			manager.SetTimer(TimerHandle, Delegate, ChargeTime, false);
		}
	}
}

void USpellAttackObject::InterruptAttack()
{
	Super::InterruptAttack();
	FTimerManager& manager = GetWorld()->GetTimerManager();
	manager.ClearTimer(TimerHandle);
}

void USpellAttackObject::CastSpell()
{
	if (IsValid(SpellClass))
	{
		UActorPoolingSubsystem* PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
		ABaseSpell* Spell = Cast<ABaseSpell>(PoolingSystem->GetActorFromPool(SpellClass));
		Spell->SetActorLocationAndRotation(OwningActor->GetActorLocation(), FRotator(0, 0, 0));
		Spell->SetOwner(OwningActor);
		Spell->SetInstigator(Cast<APawn>(OwningActor));
		Spell->InitSpell(TargetActor->GetActorLocation(), Cast<APawn>(OwningActor));
		Spell->OnSpellHitDelegate.AddDynamic(this, &USpellAttackObject::OnSpellHit);

		FTimerManager& manager = GetWorld()->GetTimerManager();

		if (CooldownTime <= 0)
		{
			CooldownCompleted();
		}
		else
		{
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &USpellAttackObject::CooldownCompleted);
			manager.SetTimer(TimerHandle, Delegate, ChargeTime, false);
		}
	}
}

void USpellAttackObject::CooldownCompleted()
{
	OnAttackCompletedDelegate.Broadcast(this);
}

void USpellAttackObject::OnSpellHit(ABaseSpell* Spell, AActor* HitActor)
{
	OnAttackHitDelegate.Broadcast(this, Spell, HitActor);
}
