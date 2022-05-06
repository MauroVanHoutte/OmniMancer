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

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditAnywhere)
	float WallWidth = 5.f;
};
