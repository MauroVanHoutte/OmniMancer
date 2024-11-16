// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/ModularAttacks/ModularAttackEffect/ModularAttackEffect.h"
#include "SpellCastAttackEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USpellCastAttackEffect : public UModularAttackEffectBase
{
	GENERATED_BODY()

public:
	virtual void OnBeginPlay(AActor* Owner) override;
	virtual void OnEndPlay() override;
	virtual void ExecuteEffect(AActor* TargetActors, const FVector& TargetLocations) override;
	virtual void InterruptEffect() override;

private:
	UFUNCTION()
	void OnSpellHit(class ABaseSpell* Spell, class AActor* HitActor);
	UFUNCTION()
	void CastSpell();
	UFUNCTION()
	void CastSingleSpell();

	UPROPERTY(EditAnywhere, NoClear)
	TSubclassOf<class ABaseSpell> SpellClass;
	UPROPERTY(EditAnywhere)
	bool bAimDuringCasting = true;
	// If true TargetActors locations are used instead of provided locations
	UPROPERTY(EditAnywhere)
	bool bUseActorsAsTarget = false;
	UPROPERTY(EditAnywhere)
	float CastingTime = 0.5f;
	UPROPERTY(EditAnywhere)
	float CooldownTime = 0.5f;
	UPROPERTY(EditAnywhere)
	bool bCastAtAllTargetsSimultaneously = true;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bCastAtAllTargetsSimultaneously"))
	bool bRandomizeTargetOrder = true;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bCastAtAllTargetsSimultaneously"))
	float CastInterval = 0.1f;
	
	FTimerHandle TimerHandle;
	UPROPERTY(Transient)
	AActor* TargettedActor;
	UPROPERTY(Transient)
	TArray<AActor*> TargetActors;
	UPROPERTY(Transient)
	TArray<FVector> TargetLocations;
	UPROPERTY(Transient)
	TArray<FVector> FinalTargets;
};
