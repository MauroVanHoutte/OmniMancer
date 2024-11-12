// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellCasting/Spells/BaseSpell.h"
#include "Explosion.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AExplosion : public ABaseSpell
{
	GENERATED_BODY()

public:
	AExplosion();

	virtual void InitSpell(const FVector& targetLocation, APawn* caster) override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnHit(AActor* hitActor) override;

private:
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* CollisionSphere;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly)
	float GrowDuration = 0.6f;
	UPROPERTY(EditDefaultsOnly)
	float LingerDuration = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float PushStrength = 10000.f;
	UPROPERTY(EditDefaultsOnly)
	float AddedUpwardsDirection = 0.4f;
	float FullSphereSize = 0.f;
	float FullMeshScale = 0.f;
	float SphereGrowSpeed = 0.f;
	float MeshGrowSpeed = 0.f;
	float Timer = 0.f;
};
