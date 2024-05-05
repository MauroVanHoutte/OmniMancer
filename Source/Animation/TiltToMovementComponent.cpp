// Fill out your copyright notice in the Description page of Project Settings.


#include "TiltToMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values for this component's properties
UTiltToMovementComponent::UTiltToMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;
	// ...
}

void UTiltToMovementComponent::Initialize(USceneComponent* TiltedComponent, UPawnMovementComponent* PawnMovementComponent)
{
	AffectedComponent = TiltedComponent;
	MovementComponent = PawnMovementComponent;
}

void UTiltToMovementComponent::TiltTowardsMovement(float DeltaTime)
{
	if (!IsValid(MovementComponent) && !IsValid(AffectedComponent))
		return;

	FVector ActorForward = AffectedComponent->GetAttachParent()->GetRelativeRotation().Vector();
	FVector VelocityDirection = MovementComponent->Velocity.GetSafeNormal();

	FVector Cross = FVector::CrossProduct(VelocityDirection, ActorForward);
	float Roll = Cross.Z * TiltAmount * -1;

	double Pitch = FVector::DotProduct(VelocityDirection, ActorForward) * TiltAmount;

	FRotator CurrentRotation = AffectedComponent->GetComponentRotation();
	AffectedComponent->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, FRotator(Roll, CurrentRotation.Yaw, Pitch), DeltaTime, TiltSpeed));
}

// Called every frame
void UTiltToMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TiltTowardsMovement(DeltaTime);
}

