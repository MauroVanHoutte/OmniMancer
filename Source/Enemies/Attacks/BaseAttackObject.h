// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAttackObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackComponentCompletedSignature, class UBaseAttackObject*, AttackObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackComponentHitSignature, class UBaseAttackObject*, AttackObject, class AActor*, AttackActor, class AActor*, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackComponentInterruptedSignature, class UBaseAttackObject*, AttackObject);

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class UNREALPROJECT_API UBaseAttackObject : public UObject
{
	GENERATED_BODY()

public:
	virtual void TickAttack(float DeltaTime) {};
	virtual void OnBeginPlay(AActor* Owner);
	virtual void OnEndPlay() {};

	UFUNCTION(BlueprintCallable)
	virtual bool AreAttackRequirementsMet(AActor* Target) { return true; };
	UFUNCTION(BlueprintCallable)
	virtual void InitiateAttack(AActor* Target, const FVector& Location = FVector(0, 0, 0)) { TargetActor = Target; TargetLocation = Location; };
	UFUNCTION(BlueprintCallable)
	virtual bool TryInterruptAttack();

	UFUNCTION(BlueprintCallable)
	virtual bool IsAttackInterruptable() { return bIsInterruptable; };

	UFUNCTION(BlueprintCallable)
	virtual bool WasActorHitBefore(AActor* TriggeredActor, class UPrimitiveComponent* ColliderComponent) { return true; };
	UFUNCTION(BlueprintCallable)
	virtual void OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent) {};

	UPROPERTY(BlueprintAssignable)
	FAttackComponentCompletedSignature OnAttackCompletedDelegate;
	UPROPERTY(BlueprintAssignable)
	FAttackComponentHitSignature OnAttackHitDelegate;
	UPROPERTY(BlueprintAssignable)
	FAttackComponentInterruptedSignature OnAttackInterruptedDelegate;

protected:
	virtual void InterruptAttack() { OnAttackInterruptedDelegate.Broadcast(this); };

	UPROPERTY(EditDefaultsOnly)
	bool bIsInterruptable = true;
	//Instead of location of target actor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUseLocationTarget = true;
	UPROPERTY(BlueprintReadOnly, Transient)
	AActor* OwningActor;
	UPROPERTY(BlueprintReadOnly, Transient)
	AActor* TargetActor;
	UPROPERTY(BlueprintReadOnly, Transient)
	FVector TargetLocation;
};
