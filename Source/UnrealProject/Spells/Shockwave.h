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

protected:
	void BeginPlay() override;
	void OnHit(AActor* hitActor) override;

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere)
	float InitialDamage = 5.f;
	UPROPERTY(EditAnywhere)
	float ScaleGrowth = 0.5f;
	UPROPERTY(EditAnywhere)
	float KnockbackAmount = 100.f;
};
