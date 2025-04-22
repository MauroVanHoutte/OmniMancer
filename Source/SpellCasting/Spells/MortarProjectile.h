// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPool/PooledActor.h"
#include "MortarProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class UNREALPROJECT_API AMortarProjectile : public APooledActor
{
	GENERATED_BODY()
	
public:
	AMortarProjectile();

	UFUNCTION(BlueprintCallable)
	UProjectileMovementComponent* GetProjectileMovementComp();

	void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnExplosion(const FVector& location );

protected:
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionComponent;
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	
	UPROPERTY(EditAnywhere, Category = "BaseParameter")
	float ProjectileRadius = 50.f;
	UPROPERTY(EditAnywhere, Category = "BaseParameter")
	float ProjectileSpeedZ = 1000.f;

};
