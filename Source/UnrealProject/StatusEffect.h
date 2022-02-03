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
	FStatusEffect(Type type, float interval, float value, float duration, AActor* cause);
	~FStatusEffect();

	Type EffectType{};
	float Interval{};
	float Value{};
	float Duration{};
	float Timer{};
	AActor* Cause{};
};
