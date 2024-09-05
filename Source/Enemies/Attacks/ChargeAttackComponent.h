// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackComponent.h"
#include "ChargeAttackComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UChargeAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UChargeAttackComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool AreAttackRequirementsMet() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	virtual void InitiateAttack() override;
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
	FName ColliderAttachmentTag;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* Collider;

	FTimerHandle TimerHandle;
	bool bCharging = false;
	float ChargedDistance = 0.f;
	UPROPERTY(Transient)
	TArray<AActor*> HitActors;
	FName NoCollisionProfileName = TEXT("NoCollision");
	FName OverlapProfileName = TEXT("OverlapAllDynamic");
	UPROPERTY(BlueprintReadOnly, Transient, meta=(AllowPrivateAccess = "true"))
	AActor* Target;
};
