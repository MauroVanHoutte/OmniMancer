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

	virtual void InitSpell(const FVector& targetLocation, const FVector& projectileDirection, AWizardCharacter* wizard) override;
	virtual void OnHit(ABaseCharacter* hitActor) override;
protected:
	virtual void BeginPlay() override;

private:
	const TArray<AActor*>& GetHitActors() const;
	void AddHitActors(const TArray<AActor*>& actors);

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float InitialDamage = 15.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	bool Stuns = true;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float StunDuration = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	int NrOfBounces = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float DamagePerWindLevel = 3.f;
	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float DurationPerFrost = 0.25f;
};
