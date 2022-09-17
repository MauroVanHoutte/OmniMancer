// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"

#include "BaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AParticleActor;
class UNiagaraSystem;

UCLASS()
class UNREALPROJECT_API ABaseProjectile : public ABaseSpell
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void FireInDirection(const FVector& direction);

	void OnHit(AActor* hitActor) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetExplosion(float radius, float damage);

	void SetBounces(int bounces);

	bool Initialized = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Explode();	


	USphereComponent* CollisionComponent;
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	//Amount of times the projectile will bounce to nearby enemies
	UPROPERTY(EditDefaultsOnly)
	int TotalBounces = 0;
	int BounceCount = 0;
	//Maximum distance between enemies while still able to bounce
	UPROPERTY(EditDefaultsOnly)
	float BounceRange = 700.f;
	//Pushback applied by projectile
	UPROPERTY(EditDefaultsOnly)
	float PushbackForce = 1500.f;

	bool Explosive = false;
	float ExplosionRadius = 0;
	float ExplosionDamage = 0;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AParticleActor> ParticleActorClass;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* ExplosionSystem;
};
