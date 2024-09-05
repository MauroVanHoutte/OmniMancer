// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackComponent.h"
#include "AreaAttackComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UAreaAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAreaAttackComponent();

	virtual bool AreAttackRequirementsMet() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	virtual void InitiateAttack() override;
	virtual void InterruptAttack() override;

private:
	void StartDamage();
	void EndDamage();

	UPROPERTY(EditDefaultsOnly)
	float Range = 200.f;
	UPROPERTY(EditDefaultsOnly)
	float ChargeUpTime = 0.7f;
	UPROPERTY(EditDefaultsOnly)
	float ActiveTime = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.f;
	UPROPERTY(EditDefaultsOnly)
	FName ColliderTag;

	UPROPERTY(Transient)
	TArray<class UPrimitiveComponent*> HurtBoxes;

	FTimerHandle TimerHandle;
	UPROPERTY(Transient)
	TArray<AActor*> HitActors;
	FName NoCollisionProfileName = TEXT("NoCollision");
	FName OverlapProfileName = TEXT("OverlapAllDynamic");
	UPROPERTY(BlueprintReadOnly, Transient, meta = (AllowPrivateAccess = "true"))
	AActor* Target;
};
