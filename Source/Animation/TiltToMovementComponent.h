// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TiltToMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UTiltToMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTiltToMovementComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(class USceneComponent* TiltedComponent, class UPawnMovementComponent* PawnMovementComponent);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:	
	void TiltTowardsMovement(float DeltaTime);

	UPROPERTY(Transient)
	class USceneComponent* AffectedComponent;
	UPROPERTY(Transient)
	class UPawnMovementComponent* MovementComponent;
	UPROPERTY(EditDefaultsOnly)
	float TiltAmount = 15.f;
	UPROPERTY(EditDefaultsOnly)
	float TiltSpeed = 100.f;
};
