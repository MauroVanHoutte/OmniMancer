#include "BaseDashComponent.h"
#include "Upgrades/StatUpgrades/StatComponent.h"
// Fill out your copyright notice in the Description page of ProjecDas.h"

// Sets default values for this component's properties
UBaseDashComponent::UBaseDashComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBaseDashComponent::Initialize(UCharacterMovementComponent* CharacterMovementComponent, UStatComponent* StatComponent)
{
	CharacterMovement = CharacterMovementComponent;
	Stats = StatComponent;
}

void UBaseDashComponent::TryExecuteDash()
{
	FTimerManager* TimerManager = &GetWorld()->GetTimerManager();
	if (TimerManager && !TimerManager->IsTimerActive(CooldownTimer))
	{
		ExecuteDash();
		ExecuteDashBP();
		TimerManager->SetTimer(CooldownTimer, CalculateCooldown(), false);
	}
}

float UBaseDashComponent::CalculateCooldown() const
{
	return Cooldown * (Stats ? Stats->GetDashCooldownMultiplier() : 1.f);
}
