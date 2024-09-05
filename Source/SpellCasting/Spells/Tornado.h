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

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

private:
	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Parameters", meta=(AllowPrivateAccess = true))
	float ScaleGrowth = 1.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Parameters", meta = (AllowPrivateAccess = true))
	float StartScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Parameters", meta = (AllowPrivateAccess = true))
	float Speed = 1000.f;
};
