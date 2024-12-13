// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SpringMovementMeshComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API USpringMovementMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	USpringMovementMeshComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMeshLocationAndRotation(const FVector& NewLocation, const FRotator& NewRotation);
	UFUNCTION(BlueprintCallable)
	void ResetToTarget();

	UFUNCTION(BlueprintCallable)
	void SetTarget(class USceneComponent* NewTarget);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	class USceneComponent* GetTarget();

protected:
	virtual void BeginPlay() override;

private:
	void OnRootTransformUpdated(USceneComponent* UpdatedComponent, EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport);
	UPROPERTY(BlueprintGetter = GetTarget, BlueprintSetter = SetTarget)
	class USceneComponent* TargetLocation;

	UPROPERTY(EditAnywhere, Category = "Translation")
	float TFrequency = 1.f;
	UPROPERTY(EditAnywhere, Category = "Translation")
	float TDamping = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Translation")
	float TResponse = 2.f;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RFrequency = 1.f;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RDamping = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RResponse = 2.f;

	FVector OldInputPos{};
	FVector Position{};
	FVector Velocity{};

	FQuat OldInputRot{};
	FQuat Rotation{};
	FRotator AngularVelocity{};
};
