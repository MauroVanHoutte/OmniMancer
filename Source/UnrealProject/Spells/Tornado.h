// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"

#include "Tornado.generated.h"

class UProjectileMovementComponent;

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
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	float TornadoDamage = 5.f;
	UPROPERTY(EditAnywhere)
	float ScaleGrowth = 1.3f;
	UPROPERTY(EditAnywhere)
	float StartScale = 1.f;
	UPROPERTY(EditAnywhere)
	float StunDuration = 2.f;
	UPROPERTY(EditAnywhere)
	float Speed = 1000.f;
};
