// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"

#include "Tornado.generated.h"

class UProjectileMovementComponent;
class AChainLightning;

UCLASS()
class UNREALPROJECT_API ATornado : public ABaseSpell
{
	GENERATED_BODY()
public:
	ATornado();

	virtual void Tick(float DeltaTime) override;
	void FireInDirection(const FVector& direction);

	virtual void InitSpell(const FVector& targetLocation, APawn* caster) override;

protected:
	virtual void BeginPlay() override;
	void OnHit(AActor* hitActor) override;

	void ShootLightning(AActor* targetActor);

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, Category="Base Parameters")
	float BaseDamage = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float ScaleGrowth = 1.3f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float StartScale = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float StunDuration = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float Speed = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float DamagePerFireLevel = 1.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float ScaleGrowthPerWindLevel = .2f;
	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float StunDurationPerFrostLevel = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	TSubclassOf<AChainLightning> Lightning;
};
