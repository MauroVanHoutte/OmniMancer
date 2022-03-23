// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "BaseSpell.h"

#include "MortarProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AMortarProjectile : public AActor
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
	USphereComponent* m_CollisionComponent;
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* m_ProjectileMovement;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_Mesh;

	
	UPROPERTY(EditAnywhere)
	float m_ProjectileRadius = 50.f;
	UPROPERTY(EditAnywhere)
	float m_ProjectileSpeedZ = 1000.f;

};
