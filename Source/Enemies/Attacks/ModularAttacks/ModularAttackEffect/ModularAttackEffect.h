// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularAttackEffect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackEffectHitSignature, class UModularAttackEffectBase*, AttackEffect, class AActor*, AttackActor, class AActor*, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackEffectCompletedSignature, class UModularAttackEffectBase*, AttackEffect);

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class UNREALPROJECT_API UModularAttackEffectBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnBeginPlay(AActor* Owner);
	virtual void OnEndPlay();
	virtual void TickAttack(float DeltaTime) {};
	virtual void ExecuteEffect(AActor* TargetActor, const FVector& TargetLocation) {};
	virtual void InterruptEffect() {};

	virtual bool WasActorHitBefore(AActor* TriggeredActor, class UPrimitiveComponent* ColliderComponent) { return false; };
	virtual void OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent) {};

	virtual void OnAttackCompleted();

	FAttackEffectHitSignature OnAttackEffectHitDelegate;
	FAttackEffectCompletedSignature OnAttackCompletedDelegate;

protected:
	virtual void GatherTargets(const FVector& TargetLocation, TArray<AActor*>& outTargetActors, TArray<FVector>& outTargetLocations);

	UPROPERTY(EditAnywhere, Instanced)
	class UModularAttackTargettingBase* TargettingModule;
	UPROPERTY(Transient)
	AActor* OwningActor;
};
