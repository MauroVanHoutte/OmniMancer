// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"

#include "IceZone.generated.h"

UCLASS()
class UNREALPROJECT_API AIceZone : public ABaseSpell
{
	GENERATED_BODY()
	
public:	
	AIceZone();

	//IHitTriggerInterface implementations
	virtual bool WasActorHitBefore_Implementation(class AActor* TriggeringActor, class UPrimitiveComponent* ColliderComponent) override;

	virtual void InitSpell(const FVector& targetLocation, APawn* caster) override;

	void SetRadius(float radius);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Destroyed() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* CylinderMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float CircleScale = 7.f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float ApplicationInterval = 0.2f;

private:
	FTimerHandle ApplicationTimer;
};
