// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpellCasting/Elements.h"
#include "Upgrades/Triggers/TriggerEffects.h"
#include "TriggerHandlingComponent.generated.h"

UENUM(BlueprintType)
enum class TriggerCondition : uint8
{
	OnSpellCast,
	OnBasicAttackCast,
	OnSpellHit,
	OnBasicAttackHit,
	OnTakeHit,
	OnDamageDealt,
	OnEnemyKiled,
	OnElementAdded
};

USTRUCT()
struct FTriggerArray 
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<class UBaseTriggerEffect*> TriggerArray;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UTriggerHandlingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerHandlingComponent();

	UFUNCTION(blueprintCallable)
	void Initialize(class UElementManipulationComponent* ElementComponent, class UHealthManager* HealthManager);

	UFUNCTION(BlueprintCallable)
	void AddTriggerEffect(TriggerCondition Condition, class UBaseTriggerEffect* TriggerEffect);
	UFUNCTION(BlueprintCallable)
	void RemoveTriggerEffect(TriggerCondition Condition, class UBaseTriggerEffect* TriggerEffect);
	
	UFUNCTION(BlueprintCallable)
	virtual void TriggerDamageDealt(AActor* DamageCause, AActor* DamagedActor, float Damage);
	UFUNCTION(BlueprintCallable)
	virtual void TriggerEnemyKilled(AActor* DamageCause, AActor* KilledEnemy, float Damage);
	UFUNCTION(BlueprintCallable)
	virtual void TriggerSpellCasted(class UElementManipulationComponent* CastingComponent, class UBasePlayerCast* CastObject, class ABaseSpell* Spell);
	UFUNCTION(BlueprintCallable)
	virtual void TriggerBasicAttackCasted(class UElementManipulationComponent* CastingComponent, class UBasePlayerCast* CastObject, class ABaseSpell* Spell);
	UFUNCTION(BlueprintCallable)
	virtual void TriggerSpellHit(class UElementManipulationComponent* CastingComponent, class UBasePlayerCast* CastObject, class ABaseSpell* Spell, AActor* HitActor);
	UFUNCTION(BlueprintCallable)
	virtual	void TriggerBasicAttackHit(class UElementManipulationComponent* CastingComponent, class UBasePlayerCast* CastObject, class ABaseSpell* Spell, AActor* HitActor);
	UFUNCTION(BlueprintCallable)
	virtual void TriggerDamageTaken(class UBaseHealthComponent* DamagedComponent, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable)
	virtual void TriggerElementAdded(class UElementManipulationComponent* ElementComponent, WizardElement OldElement, WizardElement NewElement);

protected:
	virtual void BeginPlay() override;
	void TriggerTriggerEffects(TriggerCondition Condition, ABaseSpell* Spell, AActor* Target, float Damage);

private:
	UPROPERTY(Transient)
	TMap<TriggerCondition, FTriggerArray> TriggerEffects;
};
