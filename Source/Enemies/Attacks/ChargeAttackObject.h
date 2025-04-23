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

	virtual bool WasActorHitBefore(AActor* TriggeredActor, class UPrimitiveComponent* ColliderComponent) override;
	virtual void OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent) override;

protected:
	virtual void InitiateAttack(AActor* Target, const FVector& Location) override;
	virtual void InterruptAttack() override;

private:
	void StartCharge();
	void CooldownCompleted();

	UPROPERTY(EditAnywhere)
	float AttackRange = 900.f;
	UPROPERTY(EditAnywhere)
	float ChargeDistance = 900.f;
	UPROPERTY(EditAnywhere)
	float ChargeSpeed = 900.f;
	UPROPERTY(EditAnywhere)
	float ChargeUpTime = 0.7f;
	UPROPERTY(EditAnywhere)
	float CooldownTime = 0.7f;
	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	UPROPERTY(EditAnywhere)
	FName ColliderTag = TEXT("ChargeAttackCollider");
	UPROPERTY(Transient)
	TArray<class UPrimitiveComponent*> HurtBoxes;
	UPROPERTY(Transient)
	TArray<AActor*> HitActors;

	FTimerHandle TimerHandle{};
	bool bCharging = false;
	float ChargedDistance = 0.f;
	FName NoCollisionProfileName = TEXT("NoCollision");
	FName OverlapProfileName = TEXT("OverlapAllDynamic");
};