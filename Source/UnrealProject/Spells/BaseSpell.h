// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../StatusEffect.h"
#include "BaseSpell.generated.h"

UCLASS(Abstract)
class UNREALPROJECT_API ABaseSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseSpell();

	void Destroy();

	void AddStatusEffect(const FStatusEffect& effect);

	virtual void Tick(float DeltaTime) override;

	virtual float GetDamage() const;
	virtual const TArray<FStatusEffect>& GetStatusEffects() const;
	UFUNCTION(BlueprintCallable)
	void SetDamage(float damage);
	virtual void SetDamageMultiplier(int damageMultiplier);

	void AddHitActor(AActor* actor);
	bool WasActorHit(AActor* actor) const;

	virtual void OnHit(AActor* hitActor);

protected:
	virtual void BeginPlay() override;

	float Damage = 0;

	UPROPERTY(EditAnywhere)
	TArray<FStatusEffect> StatusEffects{}; //status effectss get applied to entities hit

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 5;

	TArray<AActor*> HitActors{};	

private:
	UPROPERTY(VisibleAnywhere)
	int DamageMultiplier = 0;

	FTimerHandle LifeTimer{};
};
