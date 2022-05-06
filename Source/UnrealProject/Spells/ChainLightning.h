// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

#include "ChainLightning.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AChainLightning : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	AChainLightning();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float InitialDamage = 15.f;
	UPROPERTY(EditAnywhere)
	bool Stuns = true;
	UPROPERTY(EditAnywhere)
	float StunDuration = 0.5f;
	UPROPERTY(EditAnywhere)
	float MaxSpeed = 2000.f;
};
