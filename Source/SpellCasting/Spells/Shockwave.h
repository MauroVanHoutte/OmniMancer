// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"

#include "Shockwave.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AShockwave : public ABaseSpell
{
	GENERATED_BODY()
	
public:
	AShockwave();

	void Tick(float deltaTime) override;

	virtual void InitSpell(const FVector& targetLocation, APawn* caster) override;

protected:
	void BeginPlay() override;
	void OnHit(AActor* hitActor) override;

private:

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float GrowthDuration = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float LingerDuration = 0.4f;
	float GrowthTimer = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float InitialRadius = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float MaxScaleMultiplier = 20.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float KnockbackAmount = 500.f;
};
