// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "ProjectileAttackObject.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UProjectileAttackObject : public UBaseAttackObject
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

	UPROPERTY(EditDefaultsOnly, NoClear)
	TSubclassOf<class ABaseProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly)
	float ProjectileDamageOverride = -1.f;
	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 700.f;
	UPROPERTY(EditDefaultsOnly)
	float ChargeTime = 0.7f;
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 0.7f;

	FTimerHandle TimerHandle;
};
