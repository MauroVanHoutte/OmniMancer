// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimator.h"
#include "WizardCharacter.h"
#include "Engine/Engine.h"

UPlayerAnimator::UPlayerAnimator()
	: UAnimInstance()
{
}


void UPlayerAnimator::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	AActor* owner = GetOwningActor();
	AWizardCharacter* wizard = Cast<AWizardCharacter>(owner);

	if (wizard != nullptr)
	{
		FVector velocity = owner->GetVelocity();
		FVector velocity2D = FVector(velocity.X, velocity.Y, 0);
		FRotator rotation = wizard->GetMesh()->GetRelativeRotation();
		rotation.Yaw += 90.f;
		Speed = velocity.Size();
		MovementAnimationDirection = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(velocity2D.GetSafeNormal(), rotation.Vector())));
		LookYaw = rotation.Yaw -90.f;

		if (FVector2D::CrossProduct(FVector2D(velocity2D.GetSafeNormal()), FVector2D(rotation.Vector())) > 0)
		{
			MovementAnimationDirection *= -1;
		}

		GEngine->AddOnScreenDebugMessage(3, 0.5f, FColor::Red, rotation.Vector().ToString());
		GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Red, velocity2D.ToString());
		GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, FString::SanitizeFloat(MovementAnimationDirection));
	}
}
