// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimator.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UPlayerAnimator : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnimator();

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	float Speed = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float MovementAnimationDirection = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float LookYaw = 0;
};
