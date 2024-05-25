// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatusEffects/StatusEffect.h"
#include "BaseSpell.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpellHitSignature, ABaseSpell*, Spell, AActor*, HitActor);

class AWizardCharacter;
class ABaseCharacter;

UCLASS(Abstract)
class UNREALPROJECT_API ABaseSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseSpell();

	virtual void Destroy();

	UFUNCTION(BlueprintCallable)
	virtual void InitSpell(const FVector& targetLocation, APawn* caster);

	virtual void Tick(float DeltaTime) override;
	
	//virtual const TArray<FStatusEffect>& GetStatusEffects() const;
	UFUNCTION(BlueprintCallable)
	void SetBaseDamage(float damage);
	UFUNCTION(BlueprintCallable)
	float GetBaseDamage();
	UFUNCTION(BlueprintCallable)
	virtual void SetDamageMultiplier(float damageMultiplier);
	UFUNCTION(BlueprintCallable)
	float GetDamageMultiplier();
	UFUNCTION(BlueprintCallable)
	virtual float GetFinalDamage() const;

	UFUNCTION(BlueprintCallable)
	TArray<UBaseStatusEffect*>& GetStatusEffectsRef();

	UFUNCTION(BlueprintCallable)
	TArray<AActor*>& GetHitActorsRef();

	void AddHitActor(AActor* actor);
	bool WasActorHit(AActor* actor) const;

	virtual void OnHit(AActor* hitActor);

	FOnSpellHitSignature OnSpellHitDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void SetLifeTime(float lifespan);
	virtual void OnDeath();

	UPROPERTY(EditDefaultsOnly)
	bool ApplyWizardOnHitEffects = true;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 0;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UBaseStatusEffect*> StatusEffects{}; //status effects get applied to entities hit

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
