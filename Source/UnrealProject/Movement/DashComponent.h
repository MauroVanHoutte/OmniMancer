// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDashComponent.h"
#include "DashComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UDashComponent : public UBaseDashComponent
{
	GENERATED_BODY()

public:
	UDashComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void ExecuteDash() override;

private:
	bool bDashing = false;
	FVector DashDirection = FVector::ZeroVector;
	float DashTimer = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float DashDuration = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float DashSpeed = 1200.f;
};
