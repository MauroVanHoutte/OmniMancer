// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Engine/EngineTypes.h>
#include "CameraControllerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UCameraControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCameraControllerComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(USceneComponent* CameraTarget, APlayerController* PlayerController);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void OnRootTransformUpdated(USceneComponent* UpdatedComponent, EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport);
	UFUNCTION()
	void ReenableCameraLag();

	UPROPERTY(EditDefaultsOnly)
	float ActorToTargetLerp = 0.25f;

	UPROPERTY(Transient)
	USceneComponent* Target;
	UPROPERTY(Transient)
	APlayerController* Controller;
};
