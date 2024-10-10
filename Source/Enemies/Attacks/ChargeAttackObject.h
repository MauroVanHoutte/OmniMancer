// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "ChargeAttackObject.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UChargeAttackObject : public UBaseAttackObject
{
	GENERATED_BODY()
	
public:
	virtual void TickAttack(float DeltaTime) override;
	virtual void OnEndPlay() override;

	virtual bool AreAttackRequirementsMet(AActor* Target) override;

	virtual bool WasActorHitBefore(AActor* TriggeredActor) override;
	virtual void OnHitTriggered(AActor* HitActor) override;

protected:
	virtual void InitiateAttack(AActor* Target) override;
	virtual void InterruptAttack() override;

private:
	void StartCharge();
	void CooldownCompleted();

	UPROPERTY(EditDefaultsOnly)
	float Range = 900.f;
	UPROPERTY(EditDefaultsOnly)
	float ChargeSpeed = 900.f;
	UPROPERTY(EditDefaultsOnly)
	float ChargeUpTime = 0.7f;
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 0.7f;
	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly)
	FName ColliderTag = TEXT("ChargeAttackCollider");
	UPROPERTY(Transient)
	TArray<class UPrimitiveComponent*> HurtBoxes;
	UPROPERTY(Transient)
	TArray<AActor*> HitActors;

	FTimerHandle TimerHandle;
	bool bCharging = false;
	float ChargedDistance = 0.f;
	FName NoCollisionProfileName = TEXT("NoCollision");
	FName OverlapProfileName = TEXT("OverlapAllDynamic");
};