// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ForceApplicationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UForceApplicationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UForceApplicationComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(class UCharacterMovementComponent* MovementComp);
	UFUNCTION()
	void ApplyForce(const FVector& Force);
	UFUNCTION()
	void ApplyForceForDuration(const FVector& Force, float Duration);
	UFUNCTION()
	void ApplyImpulse(const FVector& Force);
	UFUNCTION()
	void ClearAllForces();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	class UCharacterMovementComponent* MovementComponent;
	TMap<FTimerHandle, FVector> ForcesOverDuration;
};
