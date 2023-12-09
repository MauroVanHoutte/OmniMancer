// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../StatusEffect.h"
#include "BaseSpell.generated.h"

class AWizardCharacter;
class ABaseCharacter;

UCLASS(Abstract)
class UNREALPROJECT_API ABaseSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseSpell();

	virtual void Destroy();

	void AddStatusEffect(const FStatusEffect& effect);
	UFUNCTION(BlueprintCallable)
	void SetBurnParams(bool applyBurns, float tickDamage, float tickInterval, float duration);
	UFUNCTION(BlueprintCallable)
	void SetSlowParams(bool applySlow, float value, float duration);
	UFUNCTION(BlueprintCallable)
	void SetStunParams(bool applyStun, float duration);
	UFUNCTION(BlueprintCallable)
	void SetCurseParams(bool applyCurse, float damage, float range, float duration);

	virtual void InitSpell(const FVector& targetLocation, APawn* caster);

	virtual void Tick(float DeltaTime) override;

	virtual float GetDamage() const;
	virtual const TArray<FStatusEffect>& GetStatusEffects() const;
	UFUNCTION(BlueprintCallable)
	void SetDamage(float damage);
	virtual void SetDamageMultiplier(float damageMultiplier);

	void AddHitActor(AActor* actor);
	bool WasActorHit(AActor* actor) const;

	virtual void OnHit(ABaseCharacter* hitActor);

protected:
	virtual void BeginPlay() override;
	virtual void SetLifeTime(float lifespan);
	virtual void OnDeath();

	UPROPERTY(EditDefaultsOnly)
	bool ApplyWizardOnHitEffects = true;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 0;

	UPROPERTY(EditAnywhere)
	TArray<FStatusEffect> StatusEffects{}; //status effectss get applied to entities hit

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 5;

	TArray<AActor*> HitActors{};	

	UPROPERTY(VisibleAnywhere)
	int FireLevel = -1;
	UPROPERTY(VisibleAnywhere)
	int FrostLevel = -1;
	UPROPERTY(VisibleAnywhere)
	int WindLevel = -1;

	FTimerHandle LifeTimer{};
private:
	UPROPERTY(VisibleAnywhere)
	float DamageMultiplier = 1;
	UPROPERTY(VisibleAnywhere)
	float SlowDurationMultiplier = 1;
	UPROPERTY(VisibleAnywhere)
	float BurnDurationMultiplier = 1;
	UPROPERTY(VisibleAnywhere)
	float StunDurationMultiplier = 1;

};
