// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"

#include "BaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class UNREALPROJECT_API ABaseProjectile : public ABaseSpell
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	virtual class UAffiliationComponent* GetAffiliation_Implementation() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void InitSpell(const FVector& targetLocation, APawn* caster);
	UFUNCTION(BlueprintCallable)
	void FireInDirection(const FVector& direction);

	void OnHit(AActor* hitActor) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetExplosion(float radius, float damage);

	UFUNCTION(BlueprintCallable)
	int GetBounces();
	UFUNCTION(BlueprintCallable)
	void SetBounces(int bounces);

	bool Initialized = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Explode();	

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* CollisionComponent;
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float MaxSpeed = 2000.f;

	//Amount of times the projectile will bounce to nearby enemies
	UPROPERTY(EditDefaultsOnly)
	int DefaultBounces = 0;
	int TotalBounces = DefaultBounces;
	int BounceCount = 0;
	//Maximum distance between enemies while still able to bounce
	UPROPERTY(EditDefaultsOnly)
	float BounceRange = 700.f;
	//Pushback applied by projectile
	UPROPERTY(EditDefaultsOnly)
	float PushbackForce = 1500.f;

	UPROPERTY(EditDefaultsOnly)
	bool bFireHorizontally = true;

	//Explosion
	bool Explosive = false;
	float ExplosionRadius = 0;
	float ExplosionDamage = 0;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* ExplosionParticle;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* HitParticle;
};
