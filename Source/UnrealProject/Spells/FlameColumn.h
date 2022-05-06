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

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	bool ApplyBurn = true;
	UPROPERTY(EditAnywhere)
	float BurnDamage = 1.f;
	UPROPERTY(EditAnywhere)
	float BurnInterval = 0.4f;
	UPROPERTY(EditAnywhere)
	float BurnDuration = 2.f;

	UPROPERTY(EditAnywhere)
	float InitialDamage = 5.f;

	UPROPERTY(EditAnywhere)
	float ImpactDelay = 0.8f;

	UPROPERTY(EditAnywhere)
	float VisualLinger = 0.5f;

	UPROPERTY(EditAnywhere)
	float CircleScale = 7.f;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CylinderMesh;

};
