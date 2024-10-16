// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatusEffects/StatusEffect.h"
#include "Health/HitTriggerInterface.h"
#include "BaseSpell.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpellHitSignature, ABaseSpell*, Spell, AActor*, HitActor);

class AWizardCharacter;
class ABaseCharacter;

UCLASS(Abstract)
class UNREALPROJECT_API ABaseSpell : public AActor, public IHitTriggerInterface
{
	GENERATED_BODY()
	
public:	
	ABaseSpell();

	//IHitTriggerInterface implementations
	virtual void OnTriggered_Implementation(class AActor* TriggeringActor);
	virtual class UAffiliationComponent* GetAffiliation_Implementation();
	virtual bool WasActorHitBefore_Implementation(class AActor* TriggeringActor);
	//end IHitTriggerInterface implementations

	virtual void Destroy();

	UFUNCTION(BlueprintCallable)
	virtual void InitSpell(const FVector& targetLocation, APawn* caster);

	virtual void Tick(float DeltaTime) override;
	
	//virtual const TArray<FStatusEffect>& GetStatusEffects() const;
	UFUNCTION(BlueprintCallable)
	void SetBaseDamage(float damage);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBaseDamage();
	UFUNCTION(BlueprintCallable)
	virtual void SetDamageMultiplier(float damageMultiplier);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDamageMultiplier();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetFinalDamage() const;
	UFUNCTION(BlueprintCallable)
	virtual void SetScale(float NewScale) {};
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetScale() const { return 0; };

	UFUNCTION(BLueprintCallable)
	void TrackAppliedUpgrade(class UBaseSpellUpgradeEffect* SpellUpgrade) { AppliedSpellUpgrades.Add(SpellUpgrade); };
	UFUNCTION(BlueprintCallable)
	TArray<class UBaseSpellUpgradeEffect*>& GetAppliedSpellUpgrades() { return AppliedSpellUpgrades; };

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

	TArray<class UBaseSpellUpgradeEffect*> AppliedSpellUpgrades;

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
	UPROPERTY(Transient)
	class UAffiliationComponent* AffiliationComponent;
};
