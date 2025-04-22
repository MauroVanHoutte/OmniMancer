// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ActorPoolingSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ActorPoolingSubsystem, Log, All);

USTRUCT()
struct UNREALPROJECT_API FPooledActorStruct
{
	GENERATED_BODY()

	bool bInUse = false;
	UPROPERTY(Transient)
	TObjectPtr<AActor> PooledActor;
};

USTRUCT()
struct UNREALPROJECT_API FPooledActorArray
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TArray<FPooledActorStruct> ActorArray;
};

UCLASS()
class UNREALPROJECT_API UActorPoolingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	UFUNCTION(BlueprintCallable)
	AActor* GetActorFromPool(TSubclassOf<AActor> Class);
	UFUNCTION(BlueprintCallable)
	bool ReturnActorToPool(AActor* Actor);
	UFUNCTION(BlueprintCallable)
	bool ReturnActorToPoolOrDestroy(AActor* Actor);

private:
	FPooledActorStruct& AddActorOfClassToPool(TSubclassOf<AActor> Class);
	UFUNCTION()
	void OnActorEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason);

	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AActor>, uint16> StartingActorCount;
	UPROPERTY(Transient)
	TMap<TSubclassOf<AActor>, FPooledActorArray> PooledActors;
};
