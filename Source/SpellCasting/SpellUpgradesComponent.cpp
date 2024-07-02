//Fill out your copyright notice in the Description page of Project Settings.


#include "SpellCasting/SpellUpgradesComponent.h"
#include "SpellCasting/SpellUpgradeSet.h"
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

void USpellUpgradesComponent::BeginPlay()
{

	for (TPair<WizardElement, USpellUpgradeSet*> kvp : SpellUpgradeSets)
	{
		FUpgradesArray Array;
		for (USpellUpgradeData* SpellUpgradeData : kvp.Value->AvailableUpgrades)
		{
			if (IsValid(SpellUpgradeData))
			{
				Array.UpgradesArray.Add(DuplicateObject(SpellUpgradeData, this));
			}
		}

		AvailableSpellUpgrades.Add(kvp.Key, Array);
	}

	Super::BeginPlay();
}

 void USpellUpgradesComponent::Initialize(UElementManipulationComponent* ElementManipulationComponent)
 {
	 SpellCastingComponent = ElementManipulationComponent;

	 if (IsValid(SpellCastingComponent))
		SpellCastingComponent->OnSpellCastedDelegate.AddDynamic(this, &USpellUpgradesComponent::ApplyUpgradesToSpell);
 }

 void USpellUpgradesComponent::RegisterSpellUpgrade(TSubclassOf<class ABaseSpell> SpellToApplyTo, USpellUpgradeData* Upgrade)
 {
	 AppliedSpellUpgrades.FindOrAdd(SpellToApplyTo).UpgradesArray.Add(Upgrade);
 }

 FUpgradesArray USpellUpgradesComponent::GetUpgradeSetForElement(WizardElement Element)
 {
	 return AvailableSpellUpgrades.FindOrAdd(Element);
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
		 for (TPair<UClass*, FUpgradesArray> kvp : AppliedSpellUpgrades)
		 {
			 if (Spell->IsA(kvp.Key))
			 {
				 kvp.Value.UpgradesArray.Sort([](const USpellUpgradeData& r, const USpellUpgradeData& l)
					 {
						 return r.Priority < l.Priority;
					 });
				 for (USpellUpgradeData* SpellUpgrade : kvp.Value.UpgradesArray)
				 {
					if (IsValid(SpellUpgrade) && IsValid(SpellUpgrade->Upgrade))
					{
						SpellUpgrade->Upgrade->ApplyToSpell(Spell);
						Spell->TrackAppliedUpgrade(SpellUpgrade);
					}
				 }
			 }
		 }
	 }
 }