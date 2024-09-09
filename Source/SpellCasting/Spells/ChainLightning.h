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

	virtual void InitSpell(const FVector& targetLocation, APawn* caster) override;
	virtual void OnHit(AActor* hitActor) override;
protected:
	virtual void BeginPlay() override;

private:
	const TArray<AActor*>& GetHitActors() const;
	void AddHitActors(const TArray<AActor*>& actors);
};
