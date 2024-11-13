// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "TargetAcquisitionComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UTargetAcquisitionComponent : public USphereComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Initialize(class AAIController* Controller, class UAffiliationComponent* Affiliation);
	UFUNCTION(BlueprintCallable)
	void AcquireTarget();

private:
	UPROPERTY(EditDefaultsOnly)
	FName TargetBlackboardKey;
	UPROPERTY(Transient)
	class AAIController* OwningController;
	UPROPERTY(Transient)
	class UAffiliationComponent* AffiliationComponent;
};
