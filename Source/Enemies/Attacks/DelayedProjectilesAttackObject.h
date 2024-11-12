// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "DelayedProjectilesAttackObject.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UDelayedProjectilesAttackObject : public UBaseAttackObject
{
	GENERATED_BODY()
	
public:
	UDelayedProjectilesAttackObject();

	virtual bool AreAttackRequirementsMet(AActor* Target);
	virtual void InitiateAttack(AActor* Target, const FVector& Location = FVector(0, 0, 0));

private:
	virtual void InterruptAttack() override;

	UFUNCTION()
	void StoreProjectile();
	UFUNCTION()
	void StoreAllProjectiles();
	UFUNCTION()
	void FireProjectile();
	UFUNCTION()
	void FireAllProjectiles();

	UPROPERTY(EditAnywhere, NoClear)
	TSubclassOf<class ABaseProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere)
	bool bChargeSimultaneously = false;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bChargeSimultaneously"))
	bool bChargeInOrder = false;
	UPROPERTY(EditAnywhere)
	float ChargeTime = 0.3f;
	UPROPERTY(EditAnywhere)
	int MaxStoredProjectiles = 5;
	UPROPERTY(EditAnywhere)
	bool bFireSimultaneously = false;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bFireSimultaneously"))
	bool bFireInOrder = false;
	UPROPERTY(EditAnywhere)
	float FiringDelay = 2.f;
	UPROPERTY(EditAnywhere)
	float FiringInterval = 0.5f;
	// Distance from owner's center projectiles are stored at
	UPROPERTY(EditAnywhere)
	float StorageRadius = 100.f;
	// Angle above owner between wich projectiles are stored
	UPROPERTY(EditAnywhere)
	float StorageAngle = 120;
	UPROPERTY(Transient)
	TArray<class ABaseProjectile*> StoredProjectiles;
	TArray<FVector> LocalProjectilePositions;
	FTimerHandle TimerHandle;
};
