// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/DecayingHealthComponent.h"

void UDecayingHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsDepleted && bIsDecaying)
	{
		CurrentHealth -= DecayRate * DeltaTime;

		if (CurrentHealth < 0)
		{
			CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
			bIsDepleted = true;
			bIsDecaying = false;
			OnFatalDamageTakenDelegate.Broadcast(this, 0, 0, nullptr, nullptr, nullptr);
		}
		
		if (BoundHealthbar)
			BoundHealthbar->SetHealthPercentage(CurrentHealth / MaxHealth);
	}
}

void UDecayingHealthComponent::Heal(float HealAmount)
{
	Super::Heal(HealAmount);

	bIsDecaying = false;
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UDecayingHealthComponent::StartDecaying);
	GetWorld()->GetTimerManager().SetTimer(DecayDelayTimer, Delegate, DecayDelay, false);
}

void UDecayingHealthComponent::StartDecaying()
{
	bIsDecaying = true;
}
