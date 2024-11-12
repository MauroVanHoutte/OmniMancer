// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "MortarAttackObject.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UMortarAttackObject : public UBaseAttackObject
{
	GENERATED_BODY()

public:
	virtual void OnEndPlay() override;

	virtual bool AreAttackRequirementsMet(AActor* Target) override;
	virtual void InitiateAttack(AActor* Target, const FVector& Location) override;

protected:
	virtual void InterruptAttack() override;

private:
	void FireProjectile();
	void CooldownCompleted();

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
	UPROPERTY(EditDefaultsOnly, NoClear)
	TSubclassOf<class AMortarProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, NoClear)
	TSubclassOf<class ASpellIndicator> IndicatorClass;

	FTimerHandle TimerHandle;
};