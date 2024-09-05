// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpellUpgradesComponent.generated.h"

USTRUCT(BlueprintType)
struct FSpellUpgradesArray
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<class UBaseSpellUpgradeEffect>> Upgrades;
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

	UFUNCTION(BlueprintCallable)
	void RegisterSpellUpgrade(TSubclassOf<class ABaseSpell> AffectedSpell, class UBaseSpellUpgradeEffect* Upgrade);
	UFUNCTION(BlueprintCallable)
	void RemoveSpellUpgrade(TSubclassOf<class ABaseSpell> AffectedSpell, class UBaseSpellUpgradeEffect* Upgrade);

	UFUNCTION(BlueprintCallable)
	void ApplyUpgradesToSpell(class AActor* Caster, class ABaseSpell* Spell);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(Transient)
	TMap<class UClass*, FSpellUpgradesArray> AppliedSpellUpgrades;

	UPROPERTY(Transient)
	class UElementManipulationComponent* SpellCastingComponent;
};
