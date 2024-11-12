// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"

#include "FirePool.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AFirePool : public ABaseSpell
{
	GENERATED_BODY()
public:
	AFirePool();

	virtual void InitSpell(const FVector& targetLocation, APawn* caster) override;

	void SetRadius(float radius);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnDeath() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CylinderMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float CircleScale = 4.f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float ApplicationInterval = 0.3f;

private:
	FTimerHandle ApplicationTimer;
};
