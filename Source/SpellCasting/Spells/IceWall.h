// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include <Components/BoxComponent.h>
#include "IceWall.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AIceWall : public ABaseSpell
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AIceWall();

	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	virtual void InitSpell(const FVector& targetLocation, APawn* caster) override;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Trigger = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Base Parameters")
	float WallWidth = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float WallThickness = 3.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float WallHeight = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float RiseTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float LingerTime = 1.f;

	FVector StartLocation;
	FVector TargetLocation;
	float RiseTimer = 0.f;
};
