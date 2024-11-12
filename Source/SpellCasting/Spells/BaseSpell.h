// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatusEffects/StatusEffect.h"
#include "Health/HitTriggerInterface.h"
#include "BaseSpell.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpellHitSignature, ABaseSpell*, Spell, AActor*, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpellDestroyedSignature, ABaseSpell*, Spell);

class AWizardCharacter;
class ABaseCharacter;

UCLASS(Abstract)
class UNREALPROJECT_API ABaseSpell : public AActor, public IHitTriggerInterface
{
	GENERATED_BODY()
	
public:	
	ABaseSpell();

	//IHitTriggerInterface implementations
	virtual void OnTriggered_Implementation(class AActor* TriggeringActor, class UPrimitiveComponent* ColliderComponent);
	virtual class UAffiliationComponent* GetAffiliation_Implementation();
	virtual bool WasActorHitBefore_Implementation(class AActor* TriggeringActor, class UPrimitiveComponent* CollisionComponent);
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

	UFUNCTION(BlueprintCallable)
	TArray<UBaseStatusEffect*>& GetStatusEffectsRef();

	UFUNCTION(BlueprintCallable)
	TArray<AActor*>& GetHitActorsRef();

	void AddHitActor(AActor* actor);
	bool WasActorHit(AActor* actor) const;

	virtual void OnHit(AActor* hitActor);

	FOnSpellHitSignature OnSpellHitDelegate;
	FOnSpellDestroyedSignature OnSpellDestroyedDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void OnDeath();

	UPROPERTY(EditDefaultsOnly)
	float Damage = 0;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UBaseStatusEffect*> StatusEffects{}; //status effects get applied to entities hit

	TArray<class UBaseSpellUpgradeEffect*> AppliedSpellUpgrades;
	TArray<AActor*> HitActors{};

private:
	UPROPERTY(VisibleAnywhere)
	float DamageMultiplier = 1;
	UPROPERTY(Transient)
	class UAffiliationComponent* AffiliationComponent;
};
