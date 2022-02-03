// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

#include "ChainLightning.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AChainLightning : public ABaseSpell
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AChainLightning();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& direction);

	void OnHit(AActor* hitActor) override;

private:
	USphereComponent* m_CollisionComponent;
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* m_ProjectileMovement;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_Mesh;
	UPROPERTY(EditAnywhere)
	float m_LifeTime = 2.f;
	UPROPERTY(EditAnywhere)
	float m_InitialDamage = 15.f;
	UPROPERTY(EditAnywhere)
	float m_BounceRange = 2000.f;
	UPROPERTY(EditAnywhere)
	bool m_Stuns = true;
	UPROPERTY(EditAnywhere)
	float m_StunDuration = 0.5f;
};
