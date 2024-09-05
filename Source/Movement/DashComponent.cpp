// Fill out your copyright notice in the Description page of Project Settings.


#include "DashComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Upgrades/StatUpgrades/StatComponent.h"
#include "NavigationSystem.h"


UDashComponent::UDashComponent()
{
}

void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bDashing)
	{
		//FVector Offset = DashDirection * DashSpeed * DeltaTime * (IsValid(Stats) ? Stats->GetDashForceMultiplier() : 1.f);
		
		//GetOwner()->AddActorLocalOffset(Offset);
		DashTimer += DeltaTime;
		FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, DashTimer/DashDuration);
		GetOwner()->SetActorLocation(NewLocation);
		if (DashTimer > DashDuration)
		{
			DashTimer = 0;
			bDashing = false;
		}
	}
}

void UDashComponent::ExecuteDash()
{
	DashDirection = CharacterMovement->GetLastInputVector().GetSafeNormal();
	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	EndLocation = DashDirection * DashSpeed * DashDuration + GetOwner()->GetActorLocation();
	FNavLocation OutLocation;
	if (NavSystem->ProjectPointToNavigation(EndLocation, OutLocation, FVector(2000, 2000, 2000)))
	{
		bDashing = true;
		StartLocation = GetOwner()->GetActorLocation();
		EndLocation = OutLocation.Location;
		EndLocation.Z = StartLocation.Z;
	}
}