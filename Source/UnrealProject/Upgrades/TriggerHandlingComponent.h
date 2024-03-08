// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Upgrades/Triggers/TriggerEffects.h"
#include "TriggerHandlingComponent.generated.h"

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

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void TriggerSpellCasted(AActor* Caster, class ABaseSpell* Spell);
	UFUNCTION()
	void TriggerBasicAttackCasted(AActor* Caster, class ABaseSpell* Spell);
	UFUNCTION()
	void TriggerSpellHit(class ABaseSpell* Spell, AActor* HitActor);
	UFUNCTION()
	void TriggerBasicAttackHit(class ABaseSpell* Spell, AActor* HitActor);
	UFUNCTION()
	void TriggerDamageTaken(class UBaseHealthComponent* DamagedComponent, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void TriggerTriggerEffects(TriggerCondition Condition, ABaseSpell* Spell, AActor* Target);

	UPROPERTY()
	TMap<TriggerCondition, FTriggerArray> TriggerEffects;
};
