//Fill out your copyright notice in the Description page of Project Settings.


#include "SpellCasting/SpellUpgradesComponent.h"
#include "BaseSpelUpgrade.h"
#include "ElementManipulationComponent.h"

// Sets default values for this component's properties
USpellUpgradesComponent::USpellUpgradesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

 void USpellUpgradesComponent::Initialize(UElementManipulationComponent* ElementManipulationComponent)
 {
	 SpellCastingComponent = ElementManipulationComponent;

	 if (IsValid(SpellCastingComponent))
		SpellCastingComponent->OnSpellCastedDelegate.AddDynamic(this, &USpellUpgradesComponent::ApplyUpgradesToSpell);
 }

 void USpellUpgradesComponent::RegisterSpellUpgrade(TSubclassOf<class ABaseSpell> SpellToApplyTo, UBaseSpellUpgrade* Upgrade)
 {
	 SpellUpgrades.FindOrAdd(SpellToApplyTo).UpgradesArray.Add(Upgrade);
 }

 void USpellUpgradesComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
 {
	 if (IsValid(SpellCastingComponent))
		 SpellCastingComponent->OnSpellCastedDelegate.RemoveDynamic(this, &USpellUpgradesComponent::ApplyUpgradesToSpell);
 }

 void USpellUpgradesComponent::ApplyUpgradesToSpell(AActor* Caster, ABaseSpell* Spell)
 {
	 if (IsValid(Spell))
	 {
		 for (TPair<UClass*, FUpgradesArray> kvp : SpellUpgrades)
		 {
			 if (Spell->IsA(kvp.Key))
			 {
				 kvp.Value.UpgradesArray.Sort([](const UBaseSpellUpgrade& r, const UBaseSpellUpgrade& l)
					 {
						 return r.GetPriority() < l.GetPriority();
					 });
				 for (UBaseSpellUpgrade* SpellUpgrade : kvp.Value.UpgradesArray)
				 {
					 if (IsValid(SpellUpgrade))
					 {
						 SpellUpgrade->ApplyToSpell(Spell);
					 }
				 }
			 }
		 }
	 }
 }