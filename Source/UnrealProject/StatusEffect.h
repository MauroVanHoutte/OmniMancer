// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StatusEffect.generated.h"

UENUM(BlueprintType)
enum class Type : uint8
{
	Damage,
	Slow,
	Stun,
	Curse
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FStatusEffect
{
	GENERATED_BODY()

	FStatusEffect() = default;
	FStatusEffect(Type type, float interval, float value, float duration, UObject* cause);
	~FStatusEffect();

	bool operator==(const FStatusEffect& other)
	{
		return	EffectType == other.EffectType &&
			FMath::IsNearlyEqual(Interval, other.Interval) &&
			FMath::IsNearlyEqual(Value, other.Value) &&
			FMath::IsNearlyEqual(Duration, other.Duration) &&
			FMath::IsNearlyEqual(Timer, other.Timer) &&
			Cause == other.Cause;
	};
	UPROPERTY(EditAnywhere)
	Type EffectType{};
	UPROPERTY(EditAnywhere, DisplayName = "Damage Interval/Curse spread range")
	float Interval{};
	UPROPERTY(EditAnywhere, DisplayName = "Damage amount/Slow amount")
	float Value{};
	UPROPERTY(EditAnywhere)
	float Duration{};
	float Timer{};
	UObject* Cause{};
};


