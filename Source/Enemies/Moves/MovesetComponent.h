// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovesetComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMoveComponentCompletedSignature, class UMovesetComponent*, MoveComponent, class UBaseMove*, Move);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSubMoveCompletedSignature, class UMovesetComponent*, MoveComponent, class UBaseMove*, Move);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMoveComponentHitSignature, class UMovesetComponent*, MoveComponent, class AActor*, AttackActor, class AActor*, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMoveComponentInterruptedSignature, class UMovesetComponent*, MoveComponent, class UBaseMove*, Move);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMoveComponentExecutionStartedSignature, class UMovesetComponent*, MoveComponent, class UBaseMove*, Move);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UMovesetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovesetComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual bool CanExecuteAttack(AActor* Target);
	UFUNCTION(BlueprintCallable)
	virtual class UBaseMove* ExecuteAttack(AActor* Target);
	UFUNCTION(BlueprintCallable)
	virtual bool CanBeInterrupted();
	UFUNCTION(BlueprintCallable)
	void InterruptAttack();

	UFUNCTION(BlueprintCallable)
	virtual void OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent);
	UFUNCTION(BlueprintCallable)
	virtual bool WasActorHitBefore(AActor* TestActor, class UPrimitiveComponent* ColliderComponent);

	UFUNCTION()
	void OnSubMoveCompleted(class UBaseMove* Move);

	UPROPERTY(BlueprintAssignable)
	FMoveComponentCompletedSignature OnAttackCompletedDelegate;
	UPROPERTY(BlueprintAssignable)
	FSubMoveCompletedSignature OnSubMoveCompletedDelegate;
	UPROPERTY(BlueprintAssignable)
	FMoveComponentHitSignature OnAttackHitDelegate;
	UPROPERTY(BlueprintAssignable)
	FMoveComponentInterruptedSignature OnAttackInterruptedDelegate;
	UPROPERTY(BlueprintAssignable)
	FMoveComponentExecutionStartedSignature OnMoveExecutionStartedDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

private:
	UFUNCTION()
	void OnMoveComponentCompleted(class UBaseMove* Move);
	UFUNCTION()
	void OnMoveComponentHit(class UBaseMove* Move, class AActor* AttackActor, class AActor* HitActor);
	UFUNCTION()
	void OnMoveComponentInterrupted(class UBaseMove* Move);
	UFUNCTION()
	void OnMoveExecutionStarted(class UBaseMove* Move);

	UPROPERTY(EditAnywhere, Instanced)
	TArray<class UBaseMove*> Moves;
	UPROPERTY(Transient)
	class UBaseMove* ActiveMove = nullptr;
	UPROPERTY(EditAnywhere)
	bool bAvoidRepeatedMove = true;
	UPROPERTY(Transient)
	class UBaseMove* LastUsedMove = nullptr;
};
