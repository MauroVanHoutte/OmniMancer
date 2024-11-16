// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Moves.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveCompletedSignature, class UBaseMove*, Move);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMoveHitSignature, class UBaseMove*, Move, class AActor*, AttackActor, class AActor*, HitActor);
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
	virtual void Execute(AActor* Target, const FVector& TargetLocation) {};
	UFUNCTION(BlueprintCallable)
	virtual bool CanBeExecuted(AActor* Target);
	UFUNCTION(BlueprintCallable)
	virtual bool CanBeInterrupted() { return true; };
	UFUNCTION(BlueprintCallable)
	virtual void Interrupt() {};

	UFUNCTION(BlueprintCallable)
	virtual void OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent) {};
	UFUNCTION(BlueprintCallable)
	virtual bool WasActorHitBefore(AActor* TestActor, class UPrimitiveComponent* ColliderComponent) { return false; };

	FMoveCompletedSignature OnMoveCompletedDelegate;
	FMoveHitSignature OnMoveHitDelegate;
	FMoveInterruptedSignature OnMoveInterruptedDelegate;

protected:
	UPROPERTY(EditAnywhere, Instanced)
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

	virtual void Execute(AActor* Target, const FVector& TargetLocation) override;
	virtual bool CanBeExecuted(AActor* Target) override;
	virtual bool CanBeInterrupted() override;
	virtual void Interrupt() override;

	virtual void OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent) override;
	virtual bool WasActorHitBefore(AActor* TestActor, class UPrimitiveComponent* ColliderComponent) override;

private:
	UFUNCTION()
	void OnAttackComponentCompleted(class UBaseAttackObject* Attack);
	UFUNCTION()
	void OnAttackHit(class UBaseAttackObject* Attack, class AActor* AttackActor, class AActor* HitActor);
	UFUNCTION()
	void OnAttackComponentInterrupted(class UBaseAttackObject* Attack);

	UPROPERTY(EditAnywhere)
	bool bCheckAttackRequirements = true;

	UPROPERTY(Instanced, EditAnywhere)
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

	virtual void Execute(AActor* Target, const FVector& TargetLocation) override;
	virtual bool CanBeExecuted(AActor* Target) override;
	virtual bool CanBeInterrupted() override;
	virtual void Interrupt() override;

	virtual void OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent) override;
	virtual bool WasActorHitBefore(AActor* TestActor, class UPrimitiveComponent* ColliderComponent) override;

private:
	UFUNCTION()
	void OnMoveComponentCompleted(class UBaseMove* Move);
	UFUNCTION()
	void OnMoveHit(class UBaseMove* Move, class AActor* AttackActor, class AActor* HitActor);

	UPROPERTY(EditAnywhere)
	bool bCheckFirstMoveRequirements = false;
	UPROPERTY(EditAnywhere)
	bool bCheckSubsequentMoveRequirements = false;

	UPROPERTY(Instanced, EditAnywhere)
	TArray<UBaseMove*> MoveSequence;
	int ActiveMoveIdx = -1;
	UPROPERTY(Transient)
	AActor* TargetActor = nullptr;
	UPROPERTY(Transient)
	FVector Location;
};

UCLASS()
class UNREALPROJECT_API UParallelMoves : public UBaseMove
{
	GENERATED_BODY()

public:
	virtual void TickMove(float DeltaTime) override;
	virtual void OnBeginPlay(AActor* Owner) override;
	virtual void OnEndPlay() override;

	virtual void Execute(AActor* Target, const FVector& TargetLocation) override;
	virtual bool CanBeExecuted(AActor* Target) override;
	virtual bool CanBeInterrupted() override;
	virtual void Interrupt() override;

private:
	UFUNCTION()
	void OnMoveComponentCompleted(class UBaseMove* Move);
	UFUNCTION()
	void OnMoveHit(class UBaseMove* Move, class AActor* AttackActor, class AActor* HitActor);

	UPROPERTY(Instanced, EditAnywhere)
	TArray<UBaseMove*> Moves;
	UPROPERTY(Transient)
	AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere)
	bool bCanBeInterrupted = false;

	UPROPERTY(Transient)
	TArray<UBaseMove*> ActiveMoves;
};