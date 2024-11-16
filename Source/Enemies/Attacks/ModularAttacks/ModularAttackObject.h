// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "ModularAttackObject.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UModularAttackObject : public UBaseAttackObject
{
	GENERATED_BODY()

public:
	virtual void TickAttack(float DeltaTime) override;
	virtual void OnBeginPlay(AActor* Owner) override;
	virtual void OnEndPlay() override;

	virtual bool AreAttackRequirementsMet(AActor* Target) override;
	virtual void InitiateAttack(AActor* Target, const FVector& Location) override;

	virtual bool WasActorHitBefore(AActor* TriggeredActor, class UPrimitiveComponent* ColliderComponent) override;
	virtual void OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent) override;

private:
	virtual void InterruptAttack() override;
	UFUNCTION()
	void OnEffectHit(class UModularAttackEffectBase* Effect, class AActor* AttackActor, class AActor* HitActor);
	UFUNCTION()
	void OnEffectCompleted(class UModularAttackEffectBase* Effect);

	UPROPERTY(EditAnywhere, Instanced)
	class UModularAttackRequirementBase* AttackRequirement = nullptr;
	UPROPERTY(EditAnywhere, Instanced)
	class UModularAttackEffectBase* AttackEffect = nullptr;
};
