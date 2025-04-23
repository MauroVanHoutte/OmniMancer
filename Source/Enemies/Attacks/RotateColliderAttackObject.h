// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "RotateColliderAttackObject.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API URotateColliderAttackObject : public UBaseAttackObject
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
	void StartRotation();
	UFUNCTION()
	void StartDash();
	void RotationCompleted();
	void DashCompleted();
	UFUNCTION()
	void AttackCompleted();
	FTimerHandle DashStartTimer;
	FTimerHandle RotationStartTimer;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly)
	bool bFaceTarget = true;

	UFUNCTION()
	void RemoveHitActor(AActor* Actor);
	UPROPERTY(EditDefaultsOnly)
	float MultipleHitsInterval = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 200.f;
	UPROPERTY(EditDefaultsOnly)
	float DeltaRotationInDegrees = -250.f;
	UPROPERTY(EditDefaultsOnly)
	float RotationDuration = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float RotationStartDelay = 0.f;
	float RotationTimer = 0.f;
	float RotationStartAngle = 0.f;
	bool bIsRotating = false;
	bool bRotationCompleted = false;

	UPROPERTY(EditDefaultsOnly)
	bool bRotateColliders = false;
	UPROPERTY(EditDefaultsOnly)
	float ColliderRotationSpeed = 720.f;

	UPROPERTY(EditDefaultsOnly)
	bool bDashDuringAttack = true;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bDashDuringAttack"))
	float DashDistance = 200.f;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bDashDuringAttack"))
	float DashDuration = 0.5f;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bDashDuringAttack"))
	float DashStartDelay = 0.f;
	float DashTimer = 0.f;
	bool bIsDashing = false;
	bool bDashCompleted = false;

	UPROPERTY(EditDefaultsOnly)
	float AttackCompletionDelay = 0.3f;
	FTimerHandle AttackCompletionHandle;

	UPROPERTY(EditAnywhere)
	FName ColliderTag = TEXT("SwordCollider");
	UPROPERTY(EditAnywhere)
	FName ColliderRootTag = TEXT("SwordRoot");
	FName NoCollisionProfileName = TEXT("NoCollision");
	FName OverlapProfileName = TEXT("OverlapAllDynamic");
	UPROPERTY(Transient)
	class USceneComponent* ColliderRoot = nullptr;
	UPROPERTY(Transient)
	TArray<class UPrimitiveComponent*> Colliders;
	UPROPERTY(Transient)
	TArray<AActor*> HitActors;
};
