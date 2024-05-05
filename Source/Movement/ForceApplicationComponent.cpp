// Fill out your copyright notice in the Description page of Project Settings.


#include "Movement/ForceApplicationComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 

// Sets default values for this component's properties
UForceApplicationComponent::UForceApplicationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UForceApplicationComponent::Initialize(UCharacterMovementComponent* MovementComp)
{
	MovementComponent = MovementComp;
}

void UForceApplicationComponent::ApplyForce(const FVector& Force)
{
	MovementComponent->AddForce(Force);
}

void UForceApplicationComponent::ApplyForceForDuration(const FVector& Force, float Duration)
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, Duration, false);
	ForcesOverDuration.Add(Handle, Force);
}

void UForceApplicationComponent::ApplyImpulse(const FVector& Force)
{
	MovementComponent->Velocity = FVector(0, 0, 0);
	MovementComponent->AddImpulse(Force);
}

void UForceApplicationComponent::ClearAllForces()
{
	MovementComponent->Velocity = FVector(0, 0, 0);
	ForcesOverDuration.Empty();
}

// Called when the game starts
void UForceApplicationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UForceApplicationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<FTimerHandle> ExpiredForces;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	for (auto& Tuple : ForcesOverDuration)
	{
		if (TimerManager.IsTimerActive(Tuple.Key))
		{
			MovementComponent->AddForce(Tuple.Value);
		}
		else
		{
			ExpiredForces.Add(Tuple.Key);
		}
	}

	for (FTimerHandle Handle : ExpiredForces)
	{
		ForcesOverDuration.Remove(Handle);
	}
}

