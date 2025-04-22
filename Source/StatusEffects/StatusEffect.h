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

	virtual FFormatNamedArguments GetDescriptionArguments();

	UPROPERTY(EditDefaultsOnly)
	FName Identifier;
	UPROPERTY()
	class APawn* Instigator;
	
	// Description argument is named TotalDuration
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TotalDuration = 3.f;
	UPROPERTY(VisibleAnywhere)
	float RemainingDuration = TotalDuration;
	UPROPERTY(EditDefaultsOnly)
	bool bSeperateByInstigator = true;
	UPROPERTY(EditDefaultsOnly)
	bool bRefreshDuration = true;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;
};

UCLASS(BlueprintType, EditInlineNew)
class UStunStatusEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;

private:
	FName BlackboardKey = TEXT("Stunned");
};

UCLASS(Abstract)
class UBaseStackingStatusEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void AddStack() {};

	virtual FFormatNamedArguments GetDescriptionArguments() override;

protected:
	// Description argument is named StackLimit
	UPROPERTY(EditDefaultsOnly)
	int StackLimit = -1;
	int CurrentStacks = 0;
};

UCLASS(BlueprintType, EditInlineNew)
class UMovementSpeedStatusEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

	// Description argument is named SpeedReduction(%)
	UPROPERTY(EditDefaultsOnly)
	float SpeedMultiplier = 0.5f;
};

UCLASS(BlueprintType, EditInlineNew)
class UDamageOverTimeStatusEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Update(float DeltaTime, AActor* Target) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

	// Description argument is named DamageInterval
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Interval = 0.5f;
	UPROPERTY()
	float Timer = 0.f;
	// Description argument is named TickDamage
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
	UFUNCTION()
	void OnFatalDamageTaken(UBaseHealthComponent* DamagedComponent, float DamageTaken, float OverkillDamage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	virtual FFormatNamedArguments GetDescriptionArguments() override;

	// Description argument is named CurseDamage
	UPROPERTY(EditDefaultsOnly)
	float Damage;
	// Description argument is named SpreadRange
	UPROPERTY(EditDefaultsOnly)
	float SpreadRange;
};

UCLASS(BlueprintType, EditInlineNew)
class USpellMark : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UFUNCTION()
	void OnHitTaken(AActor* HittingObject, AActor* HitActor);

	// Description argument is named MarkDamage
	UPROPERTY(EditDefaultsOnly)
	float Damage = 5;
	// Description argument is named TriggerSpell
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseSpell> TriggerSpell;
	UPROPERTY(EditDefaultsOnly)
	bool bRemoveOnTrigger = true;
};

UCLASS(BlueprintType, EditInlineNew)
class UTempCooldownReduction : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	// Description argument is named ApplicableSpellN
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<ABaseSpell>> ApplicableSpells;
	// Description argument is named CooldownReduction(%)
	UPROPERTY(EditDefaultsOnly)
	float CooldownMultiplier = 0.5f;
	UPROPERTY(Transient)
	class UElementManipulationComponent* SpellCasting;
};

UCLASS(BlueprintType, EditInlineNew)
class UExecuteEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UFUNCTION()
	void OnDamageTaken(class UBaseHealthComponent* DamagedComponent, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// Description argument is named ExecutionThreshold(%)
	UPROPERTY(EditDefaultsOnly)
	float ExecutionThreshold = 0.2f;
	UPROPERTY(Transient)
	class UHealthManager* HealthManager;
};

UCLASS()
class UChainDamageType : public UDamageType
{
	GENERATED_BODY()
};

UCLASS(BlueprintType, EditInlineNew)
class UChainDamageEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UFUNCTION()
	void OnDamageTaken(class UBaseHealthComponent* DamagedComponent, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// Description argument is named TransferPercentage(%)
	UPROPERTY(EditDefaultsOnly)
	float TransferPercentage = 0.25f;
	// Description argument is named ChainRange
	UPROPERTY(EditDefaultsOnly)
	float Range = 800.f;
	UPROPERTY(Transient)
	class UHealthManager* HealthManager;
	static TArray<TPair<UChainDamageEffect*, AActor*>> ActiveChainEffects;
};

UCLASS(BlueprintType, EditInlineNew)
class UInvulnerabilityEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;

private:
	UFUNCTION()
	void OnHitRegistered(AActor* HittingObject, AActor* HitActor);
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* Sound;
};

UCLASS(BlueprintType, EditInlineNew)
class UTemporaryStatBoostEffect : public UBaseStatusEffect
{
	GENERATED_BODY()

public:
	virtual bool Apply(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;
	virtual void Remove(AActor* Target, TArray<UBaseStatusEffect*>& ActiveEffects) override;

	virtual FFormatNamedArguments GetDescriptionArguments() override;

private:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBaseStatBoost* StatBoost;
};