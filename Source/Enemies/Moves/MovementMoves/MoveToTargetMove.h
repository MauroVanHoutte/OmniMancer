// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Moves/Moves.h"
#include "MoveToTargetMove.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UMoveToTargetMove : public UBaseMove
{
	GENERATED_BODY()
	
public:
	virtual void TickMove(float DeltaTime) override;
	virtual void Execute(AActor* Target, const FVector& TargetLocation) override;
	virtual void Interrupt() override;

private:
	bool CheckMovementComp();

	UPROPERTY(EditAnywhere)
	bool bUseSpeed = true;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseSpeed"))
	float MoveSpeed = 2500.f;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUseSpeed"))
	float TravelTime = 1.f;
	UPROPERTY(EditAnywhere)
	bool bDisableGravityForDuration = true;
	UPROPERTY(EditAnywhere)
	bool bDisableCollision = true;
	UPROPERTY(Transient)
	class UCharacterMovementComponent* MovementComp;
	FVector Location;
};
