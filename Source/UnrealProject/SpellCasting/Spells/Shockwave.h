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
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float InitialDamage = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float ScaleGrowth = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float MaxScale = 20.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float KnockbackAmount = 500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float StunDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float DamagePerWindLevel = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float KnockbackPerWindLevel = 150.f;
	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float StunDurationPerFrostLevel = 0.5f;
};
