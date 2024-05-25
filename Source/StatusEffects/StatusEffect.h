// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h" 

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
//USTRUCT()
//struct FStatusEffect
//{
//	GENERATED_BODY()
//
//	FStatusEffect() = default;
//	FStatusEffect(Type type, float interval, float value, float duration, UObject* cause);
//	~FStatusEffect();
//
//	bool operator==(const FStatusEffect& other)
//	{
//		return	EffectType == other.EffectType &&
//			FMath::IsNearlyEqual(Interval, other.Interval) &&
//			FMath::IsNearlyEqual(Value, other.Value) &&
//			FMath::IsNearlyEqual(Duration, other.Duration) &&
//			FMath::IsNearlyEqual(Timer, other.Timer) &&
//			Cause == other.Cause;
//	};
//	UPROPERTY(EditAnywhere)
//	Type EffectType{};
//	UPROPERTY(EditAnywhere, DisplayName = "Damage Interval/Curse spread range")
//	float Interval{};
//	UPROPERTY(EditAnywhere, DisplayName = "Damage amount/Slow amount")
//	float Value{};
//	UPROPERTY(EditAnywhere)
//	float Duration{};
//	float Timer{};
//	UObject* Cause{};
//};

UCLASS(Abstract, BlueprintType)
class UBaseStatusEffect : public UObject
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects);
	virtual void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) {};
	virtual void Update(float DeltaTime, AActor* Target);
	virtual bool IsExpired(AActor* Target);
	virtual void RefreshDuration() { RemainingDuration = TotalDuration; };

	virtual void SetParamsFrom(UBaseStatusEffect* StatusEffect);

	UPROPERTY(EditDefaultsOnly)
	FName Identifier;
	UPROPERTY()
	class APawn* Instigator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TotalDuration = 3.f;
	UPROPERTY(VisibleAnywhere)
	float RemainingDuration = TotalDuration;
	UPROPERTY(EditDefaultsOnly)
	bool bSeperateByInstigator = true;
	UPROPERTY(EditDefaultsOnly)
	bool bRefreshDuration = true;
};

UCLASS(Abstract)
class UBaseStackingStatusEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void AddStack() {};
};

UCLASS(BlueprintType, EditInlineNew)
class UMovementSpeedStatusEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;

	UPROPERTY(EditDefaultsOnly)
	float SpeedMultiplier = 0.5f;
};

UCLASS(BlueprintType, EditInlineNew)
class UDamageOverTimeStatusEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Update(float DeltaTime, AActor* Target) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Interval = 0.5f;
	UPROPERTY()
	float Timer = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Damage = 2.f;
};

UCLASS(BlueprintType, EditInlineNew)
class UCurseStatusEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:	
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;

	UPROPERTY(EditDefaultsOnly)
	float Damage;
	UPROPERTY(EditDefaultsOnly)
	float SpreadRange;
};

