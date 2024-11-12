// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Moves/Moves.h"
#include "LevitateMove.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ULevitateMove : public UBaseMove
{
	GENERATED_BODY()
	
public:
	virtual void TickMove(float DeltaTime) override;
	virtual void Execute(AActor* Target, const FVector& TargetLocation) override;
	virtual void Interrupt() override;

private:
	bool CheckMovementComp();
	bool CheckForceApplicationComp();
	void CallOnCompletion();
	void EnableGravity();

	UPROPERTY(EditAnywhere)
	float TargetLevitationHeight = 600.f;
	UPROPERTY(EditAnywhere)
	float UpwardsForce = 20000.f;
	UPROPERTY(EditAnywhere)
	float TimeToStayAtPeakBeforeCompletion = 2.f;
	UPROPERTY(EditAnywhere)
	float TimeToReenableAfterCompletion = 0.f;

	UPROPERTY(Transient)
	class UForceApplicationComponent* ForceApplicationComp;
	UPROPERTY(Transient)
	class UCharacterMovementComponent* MovementComp;
	float StartZLevel;
	float PeakZLevel;
	bool bRising = true;
	FTimerHandle CompletionTimerHandle;
	FTimerHandle GravityTimerHandle;
};
