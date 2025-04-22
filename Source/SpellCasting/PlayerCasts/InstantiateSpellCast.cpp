// Fill out your copyright notice in the Description page of Project Settings.

#include "InstantiateSpellCast.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include "Upgrades/StatUpgrades/StatComponent.h"
#include "SpellCasting/Spells/BaseProjectile.h"
#include "SpellCasting/Spells/BaseSpell.h"

void UInstantiateSpellCast::HandleCastInputPressed_Implementation()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.IsTimerActive(CastTimer) && !TimerManager.IsTimerActive(CooldownTimer))
	{
		OnCastStartedDelegate.Broadcast(this);
		if (CastTime > 0)
		{
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &UInstantiateSpellCast::CastSpell);
			GetWorld()->GetTimerManager().SetTimer(CastTimer, Delegate, CastTime, false);
		}
		else
		{
			CastSpell();
		}
	}
}

void UInstantiateSpellCast::HandleCastInputDown_Implementation()
{
	if (bRepeatCasting)
	{
		HandleCastInputPressed();
	}
}

void UInstantiateSpellCast::HandleCastInputReleased_Implementation()
{
}

void UInstantiateSpellCast::CastSpell()
{
	UActorPoolingSubsystem* PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
	if (IsValid(PoolingSystem))
	{
		ABaseSpell* Spell = Cast<ABaseSpell>(PoolingSystem->GetActorFromPool(SpellType));
		if (IsValid(Spell))
		{
			FVector MouseAtWizardHeight = GetMousePosAtWizardHeight();
			Spell->SetOwner(Owner);
			Spell->SetInstigator(Cast<APawn>(Owner));
			Spell->SetActorLocationAndRotation(IsValid(Origin) ? Origin->GetComponentLocation() : Owner->GetActorLocation(), FRotator(0, 0, 0));
			Spell->SetDamageMultiplier(GetDamageMultiplier());
			Spell->OnSpellHitDelegate.Clear();
			Spell->OnSpellHitDelegate.AddDynamic(this, &UInstantiateSpellCast::OnSpellHit);
			Spell->InitSpell(MouseAtWizardHeight, Cast<APawn>(Owner));

			ABaseProjectile* Projectile = Cast<ABaseProjectile>(Spell);
			if (IsValid(StatComponent) && IsValid(Projectile))
				Projectile->SetBounces(Projectile->GetBounces() + StatComponent->GetExtraBounces());

			OnCastDelegate.Broadcast(this, Spell);
			GetWorld()->GetTimerManager().SetTimer(CooldownTimer, CalculateCooldown(), false);
		}
	}
}

float UInstantiateSpellCast::CalculateCooldown()
{
	return BaseCooldown;
}

void UInstantiateSpellCast::OnSpellHit(ABaseSpell* Spell, AActor* HitActor)
{
	OnCastHitDelegate.Broadcast(this, Spell, HitActor);
}

FVector UInstantiateSpellCast::GetMousePosAtWizardHeight()
{
	FVector worldPos, worldDir;
	Controller->DeprojectMousePositionToWorld(worldPos, worldDir);
	return FMath::RayPlaneIntersection(worldPos, worldDir, FPlane(Owner->GetActorLocation(), FVector(0, 0, 1)));
}