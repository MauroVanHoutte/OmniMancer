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

	virtual void InitSpell(const FVector& casterLocation, const FVector& targetLocation, const FVector& projectileDirection, AActor* owner, APawn* instigator, int fireLevel, int frostLevel, int windLevel) override;
	virtual void OnHit(AActor* hitActor) override;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float InitialDamage = 15.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	bool Stuns = true;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float StunDuration = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float MaxSpeed = 2000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float DamagePerWindLevel = 3.f;
	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float DurationPerFrost = 0.25f;
};
