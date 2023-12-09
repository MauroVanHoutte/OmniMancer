#include "BaseDashComponent.h"
// Fill out your copyright notice in the Description page of ProjecDas.h"

// Sets default values for this component's properties
UBaseDashComponent::UBaseDashComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBaseDashComponent::TryExecuteDash()
{
	FTimerManager* TimerManager = &GetWorld()->GetTimerManager();
	if (TimerManager && !TimerManager->IsTimerActive(CooldownTimer))
	{
		ExecuteDash();
		ExecuteDashBP();
		TimerManager->SetTimer(CooldownTimer, Cooldown, false);
	}
}