// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpellUpgradesComponent.generated.h"

USTRUCT(BlueprintType)
struct FUpgradesArray
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<class USpellUpgradeData>> UpgradesArray;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API USpellUpgradesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpellUpgradesComponent();

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Initialize(class UElementManipulationComponent* SpellCastingComponent);

	//Stores and returns a duplicate of the upgrade template
	UFUNCTION(BlueprintCallable)
	void RegisterSpellUpgrade(TSubclassOf<class ABaseSpell> SpellToApplyTo, class USpellUpgradeData* Upgrade);

	UFUNCTION(BlueprintCallable)
	FUpgradesArray GetUpgradeSetForElement(WizardElement Element);

	UFUNCTION(BlueprintCallable)
	void ApplyUpgradesToSpell(class AActor* Caster, class ABaseSpell* Spell);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<class UClass*, FUpgradesArray> AppliedSpellUpgrades;

	UPROPERTY(Transient)
	class UElementManipulationComponent* SpellCastingComponent;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<WizardElement, FUpgradesArray> AvailableSpellUpgrades;

	UPROPERTY(EditDefaultsOnly)
	TMap<WizardElement, class USpellUpgradeSet*> SpellUpgradeSets;
};
