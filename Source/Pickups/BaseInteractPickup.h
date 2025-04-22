// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPool/PooledActor.h"
#include "BaseInteractPickup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickupClaimedSignature, AActor*, Player);

UCLASS(Abstract, Blueprintable)
class UNREALPROJECT_API ABaseInteractPickup : public APooledActor
{
	GENERATED_BODY()
	
public:
	ABaseInteractPickup();

	//IPooledActorInterface implementations
	virtual void OnActorTakenFromPool_Implementation() override;
	virtual void OnActorReturnedToPool_Implementation() override;
	//end IPooledActorInterface implementations

	UPROPERTY(BlueprintAssignable)
	FPickupClaimedSignature OnPickupClaimedDelegate;

protected:
	void BeginPlay() override;
	UFUNCTION(BlueprintNativeEvent)
	void OnInteractAction(AActor* Player);

	UPROPERTY(EditDefaultsOnly)
	class UInteractableComponent* InteractactableComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UWidgetComponent* Widget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> WidgetClass;

private:
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* Root;
};
