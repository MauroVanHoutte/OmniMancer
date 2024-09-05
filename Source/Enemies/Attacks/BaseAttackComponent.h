// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackComponentCompletedSignature, class UBaseAttackComponent*, AttackComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackComponentInterruptedSignature, class UBaseAttackComponent*, AttackComponent);

UCLASS( ClassGroup=(Custom), Abstract, Blueprintable, BlueprintType)
class UNREALPROJECT_API UBaseAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseAttackComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override { Super::TickComponent(DeltaTime, TickType, ThisTickFunction); };

	UFUNCTION(BlueprintCallable)
	virtual bool AreAttackRequirementsMet() { return false; };
	UFUNCTION(BlueprintCallable)
	virtual void InitiateAttack() {};
	UFUNCTION(BlueprintCallable)
	virtual bool TryInterruptAttack();

	UFUNCTION(BlueprintCallable)
	virtual bool IsAttackInterruptable() { return bIsInterruptable; };

	UPROPERTY(BlueprintAssignable)
	FAttackComponentCompletedSignature AttackCompletedDelegate;
	UPROPERTY(BlueprintAssignable)
	FAttackComponentInterruptedSignature AttackInterruptedDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	virtual void InterruptAttack() {};

	UPROPERTY(EditDefaultsOnly)
	bool bIsInterruptable = true;
};
