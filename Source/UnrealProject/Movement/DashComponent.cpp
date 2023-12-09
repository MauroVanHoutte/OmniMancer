// Fill out your copyright notice in the Description page of Project Settings.


#include "DashComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


UDashComponent::UDashComponent()
{
}

void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bDashing)
	{
		CharacterMovementComponent->AddForce(DashDirection * DashSpeed * CharacterMovementComponent->Mass);
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
	DashDirection = CharacterMovementComponent->GetLastInputVector();
}