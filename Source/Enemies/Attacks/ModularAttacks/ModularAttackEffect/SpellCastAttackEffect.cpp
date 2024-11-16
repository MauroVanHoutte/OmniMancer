// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Attacks/ModularAttacks/ModularAttackEffect/SpellCastAttackEffect.h"
#include "Algo/RandomShuffle.h"
#include "SpellCasting/Spells/BaseSpell.h"

void USpellCastAttackEffect::OnBeginPlay(AActor* Owner)
{
	Super::OnBeginPlay(Owner);
}

void USpellCastAttackEffect::OnEndPlay()
{
	Super::OnEndPlay();
}

void USpellCastAttackEffect::ExecuteEffect(AActor* TargetActor, const FVector& TargetLocation)
{
	TargetActors.Empty();
	TargetLocations.Empty();
	FinalTargets.Empty();
	TargettedActor = TargetActor;
	
	if (!bAimDuringCasting)
		GatherTargets(TargetLocation, TargetActors, TargetLocations);

	if (CastingTime > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &USpellCastAttackEffect::CastSpell);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, CastingTime, false);
	}
	else
	{
		CastSpell();
	}
}

void USpellCastAttackEffect::InterruptEffect()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void USpellCastAttackEffect::OnSpellHit(ABaseSpell* Spell, AActor* HitActor)
{
	OnAttackEffectHitDelegate.Broadcast(this, Spell, HitActor);
}

void USpellCastAttackEffect::CastSpell()
{
	if (bAimDuringCasting)
	{
		GatherTargets(IsValid(TargettedActor) ? TargettedActor->GetActorLocation() : FVector(0, 0, 0), TargetActors, TargetLocations);
	}

	if (bUseActorsAsTarget)
	{
		for (const AActor* Actor : TargetActors)
		{
			FinalTargets.Add(Actor->GetActorLocation());
		}
	}
	else
	{
		FinalTargets = TargetLocations;
	}

	if (bCastAtAllTargetsSimultaneously)
	{
		for (const FVector& Target : FinalTargets)
		{
			FActorSpawnParameters Params{};
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ABaseSpell* Spell = GetWorld()->SpawnActor<ABaseSpell>(*SpellClass, OwningActor->GetActorLocation(), FRotator(0, 0, 0), Params);
			if (IsValid(Spell))
			{
				Spell->OnSpellHitDelegate.AddDynamic(this, &USpellCastAttackEffect::OnSpellHit);
				Spell->InitSpell(Target, Cast<APawn>(OwningActor));
			}
		}

		if (CooldownTime > 0)
		{
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &UModularAttackEffectBase::OnAttackCompleted);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, CooldownTime, false);
		}
		else
		{
			OnAttackCompleted();
		}
	}
	else
	{
		if (bRandomizeTargetOrder)
		{
			Algo::RandomShuffle(FinalTargets);
		}
		CastSingleSpell();
	}
}

void USpellCastAttackEffect::CastSingleSpell()
{
	if (!FinalTargets.IsEmpty())
	{
		FVector Target;
		Target = FinalTargets.Pop();

		FActorSpawnParameters Params{};
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABaseSpell* Spell = GetWorld()->SpawnActor<ABaseSpell>(*SpellClass, OwningActor->GetActorLocation(), FRotator(0, 0, 0), Params);
		if (IsValid(Spell))
		{
			Spell->OnSpellHitDelegate.AddDynamic(this, &USpellCastAttackEffect::OnSpellHit);
			Spell->InitSpell(Target, Cast<APawn>(OwningActor));
		}

		if (!FinalTargets.IsEmpty())
		{
			if (CastInterval > 0)
			{
				FTimerDelegate Delegate;
				Delegate.BindUObject(this, &USpellCastAttackEffect::CastSingleSpell);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, CastInterval, false);
			}
			else
			{
				CastSingleSpell();
			}
			return;
		}
	}

	if (CooldownTime > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UModularAttackEffectBase::OnAttackCompleted);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, CooldownTime, false);
	}
	else
	{
		OnAttackCompleted();
	}
}
