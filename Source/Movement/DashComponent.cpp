// Fill out your copyright notice in the Description page of Project Settings.


#include "DashComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Upgrades/StatUpgrades/StatComponent.h"


UDashComponent::UDashComponent()
{
}

void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bDashing)
	{
		FVector Force = DashDirection * DashSpeed * CharacterMovement->Mass * (IsValid(Stats) ? Stats->GetDashForceMultiplier() : 1.f);
		CharacterMovement->AddForce(Force);
		DashTimer += DeltaTime;
		if (DashTimer > DashDuration)
		{
			DashTimer = 0;
			bDashing = false;
		}
	}
}

void UDashComponent::ExecuteDash()
{
	bDashing = true;
	DashDirection = CharacterMovement->GetLastInputVector().GetSafeNormal();
}