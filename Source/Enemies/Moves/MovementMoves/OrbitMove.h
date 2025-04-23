// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Moves/Moves.h"
#include "OrbitMove.generated.h"

UCLASS()
class UNREALPROJECT_API UOrbitMove : public UBaseMove
{
	GENERATED_BODY()
public:
	virtual void TickMove(float DeltaTime) override;

	virtual void Execute(AActor* Target, const FVector& TargetLocation) override;

private:
	//Positive speeds move clockwise, negative counter clockwise
	UPROPERTY(EditDefaultsOnly)
	float SidewaysSpeed = 900.f;
	UPROPERTY(EditDefaultsOnly)
	float VerticalSpeed = 1400.f;
	UPROPERTY(EditDefaultsOnly)
	float DistanceToTarget = 800.f;

	UPROPERTY(EditDefaultsOnly)
	float Duration = 3.f;
	float Timer = 0.f;
	UPROPERTY(Transient)
	AActor* TargetActor;
};
