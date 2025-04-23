// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Moves/Moves.h"
#include "BackwardsJumpMove.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UBackwardsJumpMove : public UBaseMove
{
	GENERATED_BODY()
	
public:
	virtual void TickMove(float DeltaTime) override;
	virtual void OnBeginPlay(AActor* Owner, class UMovesetComponent* MovesetComponent) override;
	virtual void OnEndPlay() override;
	virtual void Execute(AActor* Target, const FVector& TargetLocation) override;
	virtual void Interrupt() override;

private:
	UPROPERTY(EditAnywhere)
	float UpwardsForce = 1000.f;
	UPROPERTY(EditAnywhere)
	float HorizontalDistance = 600.f;
	UPROPERTY(EditAnywhere)
	float Duration = 0.8f;
	float Timer = 0.f;
	bool bActive = false;
};
