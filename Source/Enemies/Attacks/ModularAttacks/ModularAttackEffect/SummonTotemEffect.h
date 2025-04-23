// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/ModularAttacks/ModularAttackEffect/ModularAttackEffect.h"
#include "SummonTotemEffect.generated.h"

UCLASS()
class UNREALPROJECT_API USummonTotemEffect : public UModularAttackEffectBase
{
	GENERATED_BODY()

public:
	virtual void OnEndPlay() override;
	virtual void ExecuteEffect(AActor* TargetActor, const FVector& TargetLocation) override;

private:
	UFUNCTION()
	void SummonTotems();

	UPROPERTY(EditAnywhere)
	float ChargeTime = 0.7f;
	UPROPERTY(EditAnywhere)
	float CooldownTime = 0.3f;
	UPROPERTY(EditAnywhere, NoClear)
	TSubclassOf<class APawn> TotemClass;
	UPROPERTY(EditAnywhere)
	float TotemLifeTime = 8.f;
	UPROPERTY(EditAnywhere)
	bool bKillTotemsOnCasterDeath = true;
	UPROPERTY(Transient)
	TArray<AActor*> SpawnedTotems;
	UPROPERTY(Transient)
	TArray<FVector> TargetLocations;
	FTimerHandle TimerHandle;
};
