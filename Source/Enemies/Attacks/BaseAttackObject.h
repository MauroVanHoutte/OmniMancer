// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAttackObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackComponentCompletedSignature, class UBaseAttackObject*, AttackObject);
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
	virtual bool AreAttackRequirementsMet(AActor* Target) { return false; };
	UFUNCTION(BlueprintCallable)
	virtual void InitiateAttack(AActor* Target) { TargetActor = Target; };
	UFUNCTION(BlueprintCallable)
	virtual bool TryInterruptAttack();

	UFUNCTION(BlueprintCallable)
	virtual bool IsAttackInterruptable() { return bIsInterruptable; };

	UFUNCTION(BlueprintCallable)
	virtual bool WasActorHitBefore(AActor* TriggeredActor) { return false; };
	UFUNCTION(BlueprintCallable)
	virtual void OnHitTriggered(AActor* HitActor) {};

	UPROPERTY(BlueprintAssignable)
	FAttackComponentCompletedSignature OnAttackComponentCompletedDelegate;
	UPROPERTY(BlueprintAssignable)
	FAttackComponentInterruptedSignature OnAttackComponentInterruptedDelegate;

protected:
	virtual void InterruptAttack() { OnAttackComponentInterruptedDelegate.Broadcast(this); };

	UPROPERTY(EditDefaultsOnly)
	bool bIsInterruptable = true;
	UPROPERTY(BlueprintReadOnly, Transient)
	AActor* OwningActor;
	UPROPERTY(BlueprintReadOnly, Transient)
	AActor* TargetActor;
};
