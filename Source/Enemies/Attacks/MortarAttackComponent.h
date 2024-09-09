// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackComponent.h"
#include "MortarAttackComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UMortarAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()

public:
	virtual bool AreAttackRequirementsMet() override;
	virtual void InitiateAttack() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	virtual void InterruptAttack() override;

private:
	void FireProjectile();
	void CooldownCompleted();

	UPROPERTY(BlueprintReadOnly, Transient, meta = (AllowPrivateAccess = "true"))
	AActor* Target;

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 700.f;
	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 700.f;
	UPROPERTY(EditDefaultsOnly)
	float ChargeTime = 1.f;
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float IndicatorScale = 5;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AMortarProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASpellIndicator> IndicatorClass;
	FTimerHandle TimerHandle;
};
