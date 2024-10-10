// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Moves.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveCompletedSignature, class UBaseMove*, Move);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveInterruptedSignature, class UBaseMove*, Move);

UCLASS(Abstract, EditInlineNew, BlueprintType, Blueprintable)
class UNREALPROJECT_API UBaseMove : public UObject
{
	GENERATED_BODY()

public:
	virtual void TickMove(float DeltaTime) {};

	UFUNCTION(BlueprintCallable)
	virtual void OnBeginPlay(AActor* Owner) { OwningActor = Owner; };
	UFUNCTION(BlueprintCallable)
	virtual void OnEndPlay() {};
	UFUNCTION(BlueprintCallable)
	virtual void Execute(AActor* Target) {};
	UFUNCTION(BlueprintCallable)
	virtual bool CanBeExecuted(AActor* Target);
	UFUNCTION(BlueprintCallable)
	virtual bool CanBeInterrupted() { return false; };
	UFUNCTION(BlueprintCallable)
	virtual void Interrupt() {};

	UFUNCTION(BlueprintCallable)
	virtual void OnHitTriggered(AActor* HitActor) {};
	UFUNCTION(BlueprintCallable)
	virtual bool WasActorHitBefore(AActor* TestActor) { return false; };

	FMoveCompletedSignature OnMoveCompletedDelegate;
	FMoveInterruptedSignature OnMoveInterruptedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBaseMoveRequirement* ExtraRequirement;
	UPROPERTY(Transient)
	AActor* OwningActor = nullptr;
};

UCLASS()
class UNREALPROJECT_API UAttackMove : public UBaseMove
{
	GENERATED_BODY()

public:
	virtual void TickMove(float DeltaTime) override;
	virtual void OnBeginPlay(AActor* Owner) override;
	virtual void OnEndPlay() override;

	virtual void Execute(AActor* Target) override;
	virtual bool CanBeExecuted(AActor* Target) override;
	virtual bool CanBeInterrupted() override;
	virtual void Interrupt() override;

	virtual void OnHitTriggered(AActor* HitActor) override;
	virtual bool WasActorHitBefore(AActor* TestActor) override;

private:
	UFUNCTION()
	void OnAttackComponentCompleted(class UBaseAttackObject* Attack);
	UFUNCTION()
	void OnAttackComponentInterrupted(class UBaseAttackObject* Attack);

	UPROPERTY(EditDefaultsOnly)
	bool bCheckAttackRequirements = true;

	UPROPERTY(Instanced, EditDefaultsOnly)
	class UBaseAttackObject* AttackObject;
};

UCLASS()
class UNREALPROJECT_API UMoveSequence : public UBaseMove
{
	GENERATED_BODY()

public:
	virtual void TickMove(float DeltaTime) override;
	virtual void OnBeginPlay(AActor* Owner) override;
	virtual void OnEndPlay() override;

	virtual void Execute(AActor* Target) override;
	virtual bool CanBeExecuted(AActor* Target) override;
	virtual bool CanBeInterrupted() override;
	virtual void Interrupt() override;

	virtual void OnHitTriggered(AActor* HitActor) override;
	virtual bool WasActorHitBefore(AActor* TestActor) override;

private:
	UFUNCTION()
	void OnMoveComponentCompleted(class UBaseMove* Move);
	UFUNCTION()
	void OnMoveComponentInterrupted(class UBaseMove* Move);

	UPROPERTY(EditDefaultsOnly)
	bool bCheckFirstMoveRequirements = false;
	UPROPERTY(EditDefaultsOnly)
	bool bCheckSubsequentMoveRequirements = false;

	UPROPERTY(Instanced, EditDefaultsOnly)
	TArray<UBaseMove*> MoveSequence;
	int ActiveMoveIdx = -1;
	UPROPERTY(Transient)
	AActor* TargetActor = nullptr;
};