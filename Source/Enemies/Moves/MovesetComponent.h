// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovesetComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveComponentCompletedSignature, class UMovesetComponent*, MoveComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMoveComponentHitSignature, class UMovesetComponent*, MoveComponent, class AActor*, AttackActor, class AActor*, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveComponentInterruptedSignature, class UMovesetComponent*, MoveComponent);

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
	virtual bool ExecuteAttack(AActor* Target);
	UFUNCTION(BlueprintCallable)
	virtual bool CanBeInterrupted();
	UFUNCTION(BlueprintCallable)
	void InterruptAttack();

	UFUNCTION(BlueprintCallable)
	virtual void OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent);
	UFUNCTION(BlueprintCallable)
	virtual bool WasActorHitBefore(AActor* TestActor, class UPrimitiveComponent* ColliderComponent);

	UPROPERTY(BlueprintAssignable)
	FMoveComponentCompletedSignature OnAttackCompletedDelegate;
	UPROPERTY(BlueprintAssignable)
	FMoveComponentHitSignature OnAttackHitDelegate;
	UPROPERTY(BlueprintAssignable)
	FMoveComponentInterruptedSignature OnAttackInterruptedDelegate;

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

	UPROPERTY(EditAnywhere, Instanced)
	TArray<class UBaseMove*> Moves;
	UPROPERTY(Transient)
	class UBaseMove* ActiveMove = nullptr;
};
