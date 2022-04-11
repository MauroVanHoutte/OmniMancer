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
	// Sets default values for this actor's properties
	AChainLightning();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame


private:
	UPROPERTY(EditAnywhere)
	float m_InitialDamage = 15.f;
	UPROPERTY(EditAnywhere)
	bool m_Stuns = true;
	UPROPERTY(EditAnywhere)
	float m_StunDuration = 0.5f;
	UPROPERTY(EditAnywhere)
	float m_MaxSpeed = 2000.f;
};
