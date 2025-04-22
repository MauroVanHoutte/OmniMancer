// Fill out your copyright notice in the Description page of Project Settings.

#include "SpellCasting/PlayerCasts/BasePlayerCast.h"
#include "SpellCasting/ElementManipulationComponent.h"

void UBasePlayerCast::Initialize_Implementation(UElementManipulationComponent* SpellCasting, UStatComponent* Stats, AActor* CastOwner, APlayerController* CastController, USceneComponent* CastOrigin)
{
	SpellCastingComponent = SpellCasting;
	StatComponent = Stats;
	Owner = CastOwner;
	Controller = CastController;
	SetCastOrigin(CastOrigin);
}

void UBasePlayerCast::HandleCastInputPressed_Implementation()
{

}

void UBasePlayerCast::HandleCastInputDown_Implementation()
{

}

void UBasePlayerCast::HandleCastInputReleased_Implementation()
{

}

float UBasePlayerCast::GetRemainingCooldown()
{
	FTimerManager& Manager = GetWorld()->GetTimerManager();
	return Manager.IsTimerActive(CooldownTimer) ? Manager.GetTimerRemaining(CooldownTimer) : -1;
}

float UBasePlayerCast::GetTotalCooldownDuration()
{
	float CooldownMultiplier = 1.f;
	if (IsValid(SpellCastingComponent))
	{
		CooldownMultiplier = SpellCastingComponent->GetSpellCastCooldownMultiplier(this);
	}
	return BaseCooldown * CooldownMultiplier;
}

void UBasePlayerCast::SetRemainingCooldown(float NewRemainingCooldown)
{
	if (NewRemainingCooldown > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CooldownTimer, NewRemainingCooldown, false);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimer);
	}
}

float UBasePlayerCast::GetDamageMultiplier()
{
	float DamageMultiplier = 1.f;
	if (IsValid(SpellCastingComponent))
	{
		DamageMultiplier = SpellCastingComponent->GetSpellCastDamageMultiplier(this);
	}
	return DamageMultiplier;
}
