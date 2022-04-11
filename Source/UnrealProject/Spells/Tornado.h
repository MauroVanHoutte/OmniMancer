// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

#include "Tornado.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ATornado : public ABaseSpell
{
	GENERATED_BODY()
public:
	ATornado();

	virtual void Tick(float DeltaTime) override;
	void FireInDirection(const FVector& direction);
protected:
	virtual void BeginPlay() override;
	void OnHit(AActor* hitActor) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* m_ProjectileMovement;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_Mesh;
	UPROPERTY(EditAnywhere)
	float m_TornadoDamage = 5.f;
	UPROPERTY(EditAnywhere)
	float m_ScaleGrowth = 1.3f;
	UPROPERTY(EditAnywhere)
	float m_StartScale = 1.f;
	UPROPERTY(EditAnywhere)
	float m_StunDuration = 2.f;
	UPROPERTY(EditAnywhere)
	float m_Speed = 1000.f;
};
