// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include <Components/StaticMeshComponent.h>

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
	UStaticMeshComponent* m_StaticMeshComponent;
	UPROPERTY(EditAnywhere)
	float m_Lifetime = 1.f;
	UPROPERTY(EditAnywhere)
	float m_InitialDamage = 5.f;
	UPROPERTY(EditAnywhere)
	float m_ScaleGrowth = 0.5f;
	UPROPERTY(EditAnywhere)
	float m_KnockbackAmount = 100.f;
};
