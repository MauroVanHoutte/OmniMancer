// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "SpellAttackObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpellAttackHitSignature, ABaseSpell*, Spell, AActor*, HitActor);

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USpellAttackObject : public UBaseAttackObject
{
	GENERATED_BODY()
public:
	virtual void OnEndPlay() override;

	virtual void InitiateAttack(AActor* Target, const FVector& Location) override;

	FOnSpellAttackHitSignature OnSpellHitDelegate;

protected:
	virtual void InterruptAttack() override;

private:
	UFUNCTION()
	void CastSpell();
	UFUNCTION()
	void CooldownCompleted();

	UFUNCTION()
	void OnSpellHit(class ABaseSpell* Spell, class AActor* HitActor);

	UPROPERTY(EditDefaultsOnly)
	float ChargeTime = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, NoClear)
	TSubclassOf<class ABaseSpell> SpellClass;

	FTimerHandle TimerHandle;
};
