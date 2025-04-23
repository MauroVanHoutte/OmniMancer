// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "PushFromCenterAttackObject.generated.h"

UCLASS()
class UNREALPROJECT_API UPushFromCenterAttackObject : public UBaseAttackObject
{
	GENERATED_BODY()

public:
	virtual void TickAttack(float DeltaTime);
	virtual bool AreAttackRequirementsMet(AActor* Target) override;
	virtual void InitiateAttack(AActor* Target, const FVector& Location = FVector(0, 0, 0)) override;

	virtual bool WasActorHitBefore(AActor* TriggeredActor, class UPrimitiveComponent* ColliderComponent);
	virtual void OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent);

private:
	UFUNCTION()
	void StartPush();
	UFUNCTION()
	void CompleteAttack();

	UPROPERTY(EditAnywhere)
	float Damage = 4.f;
	UPROPERTY(EditAnywhere)
	float HitPushbackStrength = 2000.f;

	UPROPERTY(EditAnywhere)
	float AttackRange = 400.f;

	UPROPERTY(EditAnywhere)
	float ChargeTime = 0.8f;
	UPROPERTY(EditAnywhere)
	float PushOutDuration = 0.5f;
	UPROPERTY(EditAnywhere)
	float PushDistance = 400.f;
	float PushTimer = 0.f;
	bool bPushing = false;

	UPROPERTY(EditAnywhere)
	bool bReturnToStartPosition = false;
	UPROPERTY(EditAnywhere)
	float CooldownTime = 0.5f;

	UPROPERTY(EditAnywhere)
	FName ColliderTag = TEXT("SwordCollider");
	UPROPERTY(EditAnywhere)
	FName CenterComponentTag = TEXT("SwordRoot");
	FName NoCollisionProfileName = TEXT("NoCollision");
	FName OverlapProfileName = TEXT("OverlapAllDynamic");

	UPROPERTY(Transient)
	class USceneComponent* CenterComponent = nullptr;
	UPROPERTY(Transient)
	TArray<class UPrimitiveComponent*> Colliders;
	UPROPERTY(Transient)
	FTimerHandle TimerHandle{};
	UPROPERTY(Transient)
	TArray<AActor*> HitActors{};
};
