// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "BaseSpell.h"

#include "FlameColumn.generated.h"

UCLASS()
class UNREALPROJECT_API AFlameColumn : public ABaseSpell
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlameColumn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	bool m_ApplyBurn = true;
	UPROPERTY(EditAnywhere)
	float m_BurnDamage = 1.f;
	UPROPERTY(EditAnywhere)
	float m_BurnInterval = 0.4f;
	UPROPERTY(EditAnywhere)
	float m_BurnDuration = 2.f;

	UPROPERTY(EditAnywhere)
	float m_InitialDamage = 5.f;

	UPROPERTY(EditAnywhere)
	float m_ImpactDelay = 0.8f;

	UPROPERTY(EditAnywhere)
	float m_VisualLinger = 0.5f;

	UPROPERTY(EditAnywhere)
	float m_CircleScale = 7.f;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_CylinderMesh;

};
