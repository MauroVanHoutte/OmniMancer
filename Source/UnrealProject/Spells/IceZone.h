// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSpell.h"

#include "IceZone.generated.h"

UCLASS()
class UNREALPROJECT_API AIceZone : public ABaseSpell
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIceZone();

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

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* m_CylinderMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(EditAnywhere)
	bool m_ApplyBurn = true;
	UPROPERTY(EditAnywhere)
	float m_BurnDamage = 1.f;
	UPROPERTY(EditAnywhere)
	float m_BurnInterval = 0.5f;
	UPROPERTY(EditAnywhere)
	float m_EffectLingerDuration = 2.f;
	UPROPERTY(EditAnywhere)
	int m_SlowAmount = 50;

	UPROPERTY(EditAnywhere)
	float m_Lifetime = 4.f;

	UPROPERTY(EditAnywhere)
	float m_CircleScale = 7.f;

	UPROPERTY(EditAnywhere)
	float m_ApplicationInterval = 0.2f;

	float m_ApplicationTimer = 0.f;

	FTimerHandle m_LifetimeHandle;
};
