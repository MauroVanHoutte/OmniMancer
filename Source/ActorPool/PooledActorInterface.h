// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PooledActorInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorReturnedToPoolSignature, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorTakenFromPoolSignature, AActor*, Actor);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPooledActorInterface : public UInterface
{
	GENERATED_BODY()
};

class UNREALPROJECT_API IPooledActorInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnActorTakenFromPool();
	UFUNCTION(BlueprintNativeEvent)
	void OnActorReturnedToPool();
	UFUNCTION()
	virtual FActorReturnedToPoolSignature& GetReturnedToPoolDelegate() = 0;
	UFUNCTION()
	virtual FActorTakenFromPoolSignature& GetTakenFromPoolDelegate() = 0;
};
