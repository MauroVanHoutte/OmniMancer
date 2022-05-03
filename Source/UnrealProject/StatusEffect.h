// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StatusEffect.generated.h"

enum class Type
{
	Damage,
	Slow,
	Stun
};

/**
 * 
 */
USTRUCT()
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

	Type EffectType{};
	float Interval{};
	float Value{};
	float Duration{};
	float Timer{};
	UObject* Cause{};
};


