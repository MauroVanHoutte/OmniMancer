// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include "Vortex.generated.h"


class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AVortex : public ABaseSpell
{
	GENERATED_BODY()
public:
	AVortex();

	virtual void InitSpell(const FVector& targetLocation, const FVector& projectileDirection, AWizardCharacter* wizard) override;

	void SetRadius(float radius);

	virtual void Tick(float deltaTime) override;

protected:
	void BeginPlay() override;
	virtual void OnDeath() override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CylinderMesh;
	UPROPERTY(EditAnywhere)
	float PullStrength = 10.f;
	UPROPERTY(EditAnywhere)
	float Radius = 5.f;
	UPROPERTY(EditAnywhere)
	float FinalDamage = 5.f;
};
