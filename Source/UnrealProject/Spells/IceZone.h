// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"

#include "IceZone.generated.h"

UCLASS()
class UNREALPROJECT_API AIceZone : public ABaseSpell
{
	GENERATED_BODY()
	
public:	
	AIceZone();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetBurnParams(bool applyBurns, float tickDamage, float tickInterval, float duration);
	UFUNCTION(BlueprintCallable)
	void SetSlowParams(bool applySlow, float value, float duration);
	void SetRadius(float radius);
	void SetLifetime(float lifetime);

	void Destroy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CylinderMesh;

	UPROPERTY(EditAnywhere)
	bool ApplyBurn = true;
	UPROPERTY(EditAnywhere)
	float BurnDamage = 1.f;
	UPROPERTY(EditAnywhere)
	float BurnInterval = 0.5f;
	UPROPERTY(EditAnywhere)
	float EffectLingerDuration = 2.f;
	UPROPERTY(EditAnywhere)
	int SlowAmount = 50;

	UPROPERTY(EditAnywhere)
	float CircleScale = 7.f;

	UPROPERTY(EditAnywhere)
	float ApplicationInterval = 0.2f;

private:

	float ApplicationTimer = 0.f;

	FTimerHandle LifetimeHandle;
};
