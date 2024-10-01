// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "AreaAttackObject.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UAreaAttackObject : public UBaseAttackObject
{
	GENERATED_BODY()

public:
	virtual bool AreAttackRequirementsMet(AActor* Target) override;

	virtual bool WasActorHitBefore(AActor* TriggeredActor) override;
	virtual void OnHitTriggered(AActor* HitActor) override;

protected:
	virtual void InitiateAttack(AActor* Target) override;
	virtual void InterruptAttack() override;

private:
	void StartDamage();
	void EndDamage();
	void AttackCompleted();

	UPROPERTY(EditDefaultsOnly)
	float Range = 200.f;
	UPROPERTY(EditDefaultsOnly)
	float ChargeUpTime = 0.7f;
	UPROPERTY(EditDefaultsOnly)
	float AttackCompletionDelay = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float ActiveTime = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly)
	FName ColliderTag = TEXT("AreaAttackCollider");
	UPROPERTY(Transient)
	TArray<class UPrimitiveComponent*> HurtBoxes;
	UPROPERTY(Transient)
	TArray<AActor*> HitActors;

	FTimerHandle DamageTimerHandle;
	FTimerHandle CompletionTimerHandle;
	FName NoCollisionProfileName = TEXT("NoCollision");
	FName OverlapProfileName = TEXT("OverlapAllDynamic");
};