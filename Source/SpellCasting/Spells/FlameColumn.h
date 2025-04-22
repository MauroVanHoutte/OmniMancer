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

	//IPooledActorInterface implementations
	virtual void OnActorTakenFromPool_Implementation() override;
	//end IPooledActorInterface implementations

	virtual void Tick(float DeltaTime) override;

	virtual void InitSpell(const FVector& targetLocation, APawn* caster) override;

	virtual void SetScale(float NewScale) override;
	virtual float GetScale() const override { return CircleScale; };

	virtual void OnHit(AActor* hitActor) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float ImpactDelay = 0.8f;
	float ImpactTimer = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float VisualLinger = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float DefaultCircleScale = 7.f;
	float CircleScale = DefaultCircleScale;

	bool SetOff = false;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CylinderMesh;
};
