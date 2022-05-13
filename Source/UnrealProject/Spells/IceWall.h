// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
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

	virtual void InitSpell(const FVector& casterLocation, const FVector& targetLocation, const FVector& projectileDirection, AActor* owner, APawn* instigator, int fireLevel, int frostLevel, int windLevel) override;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Base Parameters")
	float WallWidth = 5.f;
};
