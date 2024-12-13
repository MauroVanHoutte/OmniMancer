// Fill out your copyright notice in the Description page of Project Settings.

#include "Movement/CameraControllerComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UCameraControllerComponent::UCameraControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCameraControllerComponent::Initialize(USceneComponent* CameraTarget, APlayerController* PlayerController)
{
	Target = CameraTarget;
	Controller = PlayerController;
	GetOwner()->GetRootComponent()->TransformUpdated.AddUObject(this, &UCameraControllerComponent::OnRootTransformUpdated);
}

// Called every frame
void UCameraControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Actor = GetOwner();

	FVector CursorLocation;
	FVector CursorDirection;
	if (IsValid(Target) && IsValid(Controller) && Controller->DeprojectMousePositionToWorld(CursorLocation, CursorDirection))
	{
		FVector MouseAtPlayerHeight = FMath::RayPlaneIntersection(CursorLocation, CursorDirection, UE::Math::TPlane(Actor->GetActorLocation(), FVector(0, 0, 1)));
		FVector Center = FMath::Lerp(Actor->GetActorLocation(), MouseAtPlayerHeight, ActorToTargetLerp);
		Target->SetWorldLocation(Center);
	}
	else
	{
		Target->SetWorldLocation(Actor->GetActorLocation());
	}
}

void UCameraControllerComponent::OnRootTransformUpdated(USceneComponent* UpdatedComponent, EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	if (Teleport == ETeleportType::ResetPhysics)
	{
		USpringArmComponent* SpringArm = GetOwner()->GetComponentByClass<USpringArmComponent>();
		if (IsValid(SpringArm))
		{
			SpringArm->bEnableCameraLag = false;
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &UCameraControllerComponent::ReenableCameraLag);
			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, 0.1, false);
		}
	}
}

void UCameraControllerComponent::ReenableCameraLag()
{
	USpringArmComponent* SpringArm = GetOwner()->GetComponentByClass<USpringArmComponent>();
	if (IsValid(SpringArm))
	{
		SpringArm->bEnableCameraLag = true;
	}
}

