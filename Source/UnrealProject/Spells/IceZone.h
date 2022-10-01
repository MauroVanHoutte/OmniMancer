// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"

#include "IceZone.generated.h"

UCLASS()
class UNREALPROJECT_API AIceZone : public ABaseSpell
{
	GENERATED_BODY()
	
public:	
	AIceZone();

	virtual void Tick(float DeltaTime) override;

	virtual void InitSpell(const FVector& casterLocation, const FVector& targetLocation, const FVector& projectileDirection, AActor* owner, APawn* instigator, int fireLevel, int frostLevel, int windLevel) override;

	void SetRadius(float radius);

	void Destroy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* CylinderMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	bool ApplyBurn = true;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float BurnDamage = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float BurnInterval = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float EffectLingerDuration = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	int SlowAmount = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float CircleScale = 7.f;

	UPROPERTY(EditDefaultsOnly, Category = "Base Parameters")
	float ApplicationInterval = 0.2f;


	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float DamagePerFireLevel = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float SlowPerFrostLevel = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Element Scaling")
	float DurationPerFrostLevel = 0.f;

private:

	float ApplicationTimer = 0.f;

};
