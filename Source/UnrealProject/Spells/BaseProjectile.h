// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/StaticMeshComponent.h>
#include <GameplayTagContainer.h>
#include "../ParticleActor.h"

#include "BaseProjectile.generated.h"

UCLASS()
class UNREALPROJECT_API ABaseProjectile : public ABaseSpell
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Explode();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void FireInDirection(const FVector& direction);

	void OnHit(AActor* hitActor) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetExplosion(float radius, float damage);

	void SetBounces(int bounces);

protected:
	USphereComponent* m_CollisionComponent;
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* m_ProjectileMovement;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_Mesh;

	//Amount of times the projectile will bounce to nearby enemies
	UPROPERTY(EditDefaultsOnly)
	int m_TotalBounces = 0;
	int m_BouceCount = 0;
	//Maximum distance between enemies while still able to bounce
	UPROPERTY(EditDefaultsOnly)
	float m_BounceRange = 700.f;

	bool m_Explosive = false;
	float m_ExplosionRadius = 0;
	float m_ExplosionDamage = 0;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AParticleActor> m_ParticleActorClass;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* m_ExplosionSystem;
};
