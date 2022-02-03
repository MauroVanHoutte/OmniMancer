// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffect.h"

FStatusEffect::FStatusEffect(Type type, float interval, float value, float duration, AActor* cause)
	: EffectType{type}
	, Interval{interval}
	, Value{value}
	, Duration{duration}
	, Cause{cause}
{
}

FStatusEffect::~FStatusEffect()
{
}
