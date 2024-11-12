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
	float ImpactDelay = 0.8f;
	float ImpactTimer = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float VisualLinger = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float CircleScale = 7.f;

	bool SetOff = false;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CylinderMesh;
};
