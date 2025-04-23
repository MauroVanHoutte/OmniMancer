// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "LaunchColliderAndRecall.generated.h"

UCLASS()
class UNREALPROJECT_API ULaunchColliderAndRecall : public UBaseAttackObject
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
	void StartLaunch();
	void LaunchCompleted();
	UFUNCTION()
	void StartRecall();
	void RecallCompleted();
	FVector CalculateLaunchTarget();

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 900.f;
	float Damage = 8.f;
	UPROPERTY(EditDefaultsOnly)
	float ChargeTime = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float RecallDelay = 1.5f;
	UPROPERTY(EditDefaultsOnly)
	float LaunchSpeed = 1200.f;
	UPROPERTY(EditDefaultsOnly)
	float OvershootDistance = 200.f;
	UPROPERTY(EditDefaultsOnly)
	float RecallSpeed = 900.f;
	bool bHitDuringRecall = true;
	bool bLaunching = false;
	bool bRecalling = false;

	FName NoCollisionProfileName = TEXT("NoCollision");
	FName OverlapProfileName = TEXT("OverlapAllDynamic");

	UPROPERTY(EditDefaultsOnly)
	FName LaunchedColliderTag = NAME_None;
	UPROPERTY(Transient)
	FVector LaunchLocation;
	UPROPERTY(Transient)
	FTransform PreLaunchTransform;
	UPROPERTY(Transient)
	USceneComponent* Parent;
	UPROPERTY(Transient)
	class UPrimitiveComponent* LaunchedCollider;
	UPROPERTY(Transient)
	TArray<AActor*> HitActors;
	FTimerHandle TimerHandle{};
};
