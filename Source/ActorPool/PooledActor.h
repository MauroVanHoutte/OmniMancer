// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPool/PooledActorInterface.h"
#include "GameFramework/Actor.h"
#include "PooledActor.generated.h"

UCLASS()
class UNREALPROJECT_API APooledActor : public AActor, public IPooledActorInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool ReturnToPoolOrDestroy();
	UFUNCTION(BlueprintCallable)
	static APooledActor* GetActorFromPool(UWorld* World);

	//IPooledActorInterface implementations
	virtual void OnActorTakenFromPool_Implementation() override;
	virtual void OnActorReturnedToPool_Implementation() override;
	virtual FActorReturnedToPoolSignature& GetReturnedToPoolDelegate() override;
	virtual FActorTakenFromPoolSignature& GetTakenFromPoolDelegate() override;
	//end IPooledActorInterface implementations

	UPROPERTY(BlueprintAssignable)
	FActorReturnedToPoolSignature OnActorReturnedToPoolDelegate;
	UPROPERTY(BlueprintAssignable)
	FActorTakenFromPoolSignature OnActorTakenFromPoolDelegate;

protected:
	void EnableComponents();
	void DisableComponents();

	UPROPERTY(Transient)
	class UActorPoolingSubsystem* PoolingSystem;
};
