// Fill out your copyright notice in the Description page of Project Settings.


#include "SpringMovementMeshComponent.h"

USpringMovementMeshComponent::USpringMovementMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpringMovementMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DeltaTime = FMath::Clamp(DeltaTime, 0, 0.1f);

	//Position and Speed

	float k1 = TDamping / (PI * TFrequency);
	float k2 = 1 / FMath::Pow((2 * PI * TFrequency), 2);
	float k3 = (TResponse * TDamping) / (2 * PI * TFrequency);

	FVector TargetPos = TargetLocation ? TargetLocation->GetComponentLocation() : FVector(0, 0, 0);
	if (!FVector::PointsAreNear(Position, TargetPos, 0.1f) || !Velocity.IsNearlyZero())
	{
		FVector inputSpeed = (TargetPos - OldInputPos) / DeltaTime; //estimate input speed

		FVector newPosition = Position + Velocity * DeltaTime;
		FVector newSpeed = Velocity + DeltaTime * (TargetPos + k3 * inputSpeed - newPosition - k1 * Velocity) / k2;

		Position = newPosition;
		Velocity = newSpeed;
		OldInputPos = TargetPos;

		SetWorldLocation(Position);
	}

	//Rotation

	k1 = RDamping / (PI * RFrequency);
	k2 = 1 / FMath::Pow((2 * PI * RFrequency), 2);
	k3 = (RResponse * RDamping) / (2 * PI * RFrequency);

	FQuat targetRotation = TargetLocation->GetComponentRotation().Quaternion();

	if (!(Rotation.Equals(targetRotation)) || !AngularVelocity.IsNearlyZero())
	{
		FQuat deltaRotation = targetRotation - OldInputRot;
		FQuat inputRotationVelocity = deltaRotation * (1 / DeltaTime); //estimate input rotation speed

		FQuat newRotation = Rotation + (AngularVelocity * DeltaTime).Quaternion();

		FRotator newAngularVelocity = AngularVelocity + DeltaTime * (targetRotation.Rotator() + k3 * inputRotationVelocity.Rotator() - newRotation.Rotator() - k1 * AngularVelocity) * (1 / k2);

		Rotation = newRotation;
		AngularVelocity = newAngularVelocity;
		OldInputRot = targetRotation;

		SetWorldRotation(Rotation);
	}
}

void USpringMovementMeshComponent::SetMeshLocationAndRotation(const FVector& NewLocation, const FRotator& NewRotation)
{
	SetWorldLocationAndRotation(NewLocation, NewRotation);
	Position = NewLocation;
	Rotation = NewRotation.Quaternion();
}

void USpringMovementMeshComponent::SetTarget(USceneComponent* NewTarget)
{
	TargetLocation = NewTarget;
}

USceneComponent* USpringMovementMeshComponent::GetTarget()
{
	return TargetLocation;
}

void USpringMovementMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(TargetLocation))
	{
		FTransform Transform = TargetLocation->GetComponentTransform();
		SetMeshLocationAndRotation(Transform.GetLocation(), Transform.GetRotation().Rotator());
		OldInputPos = Position;
		OldInputRot = Rotation;
	}
}
