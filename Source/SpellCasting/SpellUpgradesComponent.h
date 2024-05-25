// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpellUpgradesComponent.generated.h"

USTRUCT()
struct FUpgradesArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<UBaseSpellUpgrade>> UpgradesArray;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API USpellUpgradesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpellUpgradesComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(class UElementManipulationComponent* SpellCastingComponent);

	UFUNCTION(BlueprintCallable)
	void RegisterSpellUpgrade(class UClass* SpellToApplyTo, class UBaseSpellUpgrade* Upgrade);

	UFUNCTION(BlueprintCallable)
	void ApplyUpgradesToSpell(class AActor* Caster, class ABaseSpell* Spell);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(Transient)
	TMap<class UClass*, FUpgradesArray> SpellUpgrades;

	UPROPERTY(Transient)
	class UElementManipulationComponent* SpellCastingComponent;
};
