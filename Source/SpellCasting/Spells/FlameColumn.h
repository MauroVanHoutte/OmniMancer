// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"

#include "FlameColumn.generated.h"

UCLASS()
class UNREALPROJECT_API AFlameColumn : public ABaseSpell
{
	GENERATED_BODY()
	
public:	
	AFlameColumn();

	virtual void Tick(float DeltaTime) override;

	virtual void InitSpell(const FVector& targetLocation, APawn* caster) override;

	virtual void SetScale(float NewScale) override { CircleScale = NewScale; };
	virtual float GetScale() const override { return CircleScale; };

	virtual void OnHit(AActor* hitActor) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnDeath() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	bool ApplyBurn = true;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float BurnDamage = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float BurnInterval = 0.4f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float BurnDuration = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float ImpactDelay = 0.8f;
	float ImpactTimer = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float VisualLinger = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float CircleScale = 7.f;

	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float DamagePerFireLevel = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float BurnDamagePerFireLevel = 0.3f;
	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float ScalePerWindLevel = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float DurationPerFrostLevel = 0.5f;

	bool SetOff = false;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CylinderMesh;
	UPROPERTY(EditAnywhere)
	FTimerHandle DoubleCast;
};
