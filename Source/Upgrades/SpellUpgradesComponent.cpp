//Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrades/SpellUpgradesComponent.h"
#include "SpellUpgrades/BaseSpelUpgrade.h"
#include "SpellCasting/ElementManipulationComponent.h"

// Sets default values for this component's properties
USpellUpgradesComponent::USpellUpgradesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USpellUpgradesComponent::BeginPlay()
{
	Super::BeginPlay();
}

 void USpellUpgradesComponent::Initialize(UElementManipulationComponent* ElementManipulationComponent)
 {
	 SpellCastingComponent = ElementManipulationComponent;

	 if (IsValid(SpellCastingComponent))
	 {
		 SpellCastingComponent->OnSpellCastedDelegate.AddDynamic(this, &USpellUpgradesComponent::ApplyUpgradesToSpell);
		 SpellCastingComponent->OnBasicAttackCastedDelegate.AddDynamic(this, &USpellUpgradesComponent::ApplyUpgradesToSpell);
		 SpellCastingComponent->OnTriggeredSpellCastedDelegate.AddDynamic(this, &USpellUpgradesComponent::ApplyUpgradesToSpell);
	 }
 }

 void USpellUpgradesComponent::RegisterSpellUpgrade(TSubclassOf<class ABaseSpell> AffectedSpell, UBaseSpellUpgradeEffect* Upgrade)
 {
	 AppliedSpellUpgrades.FindOrAdd(AffectedSpell).Upgrades.Add(Upgrade);
 }

 void USpellUpgradesComponent::RemoveSpellUpgrade(TSubclassOf<class ABaseSpell> AffectedSpell, UBaseSpellUpgradeEffect* Upgrade)
 {
	 FSpellUpgradesArray* Array = AppliedSpellUpgrades.Find(AffectedSpell);
	 if (Array)
	 {
		 Array->Upgrades.Remove(Upgrade);
		 Upgrade->OnUpgradeRemoved(this, AffectedSpell);
	 }
 }

 void USpellUpgradesComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
 {
	 Super::EndPlay(EndPlayReason);

	 if (IsValid(SpellCastingComponent))
		 SpellCastingComponent->OnSpellCastedDelegate.RemoveDynamic(this, &USpellUpgradesComponent::ApplyUpgradesToSpell);
 }

 void USpellUpgradesComponent::ApplyUpgradesToSpell(AActor* Caster, ABaseSpell* Spell)
 {
	 if (IsValid(Spell))
	 {
		 for (TPair<UClass*, FSpellUpgradesArray> kvp : AppliedSpellUpgrades)
		 {
			 if (Spell->IsA(kvp.Key))
			 {
				 kvp.Value.Upgrades.Sort([](const UBaseSpellUpgradeEffect& r, const UBaseSpellUpgradeEffect& l)
					 {
						 return r.Priority < l.Priority;
					 });
				 for (UBaseSpellUpgradeEffect* SpellUpgrade : kvp.Value.Upgrades)
				 {
					if (IsValid(SpellUpgrade))
					{
						SpellUpgrade->ApplyToSpell(Spell);
						Spell->TrackAppliedUpgrade(SpellUpgrade);
					}
				 }
			 }
		 }
	 }
 }