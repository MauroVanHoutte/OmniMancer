// Fill out your copyright notice in the Description page of Project Settings.

#include "Upgrades/CharacterUpgradesComponent.h"
#include "Upgrades/CharacterUpgradeSet.h"
#include "Upgrades/CharacterUpgrades.h"

// Sets default values for this component's properties
UCharacterUpgradesComponent::UCharacterUpgradesComponent()
{
}

void UCharacterUpgradesComponent::BeginPlay()
{
	for (auto kvp : LevelUpUpgradeSets)
	{
		FUpgradesArray& UpgradesArray = AvailableLevelUpUpgrades.FindOrAdd(kvp.Key);
		for (const UCharacterUpgrade* Upgrade : kvp.Value->Upgrades)
		{
			UCharacterUpgrade* UpgradeDuplicate = DuplicateObject(Upgrade, GetOwner());
			UpgradesArray.Upgrades.Add(UpgradeDuplicate);
			if (UpgradeDuplicate->bStartActive)
				ApplyUpgrade(UpgradeDuplicate);
		}
	}

	Super::BeginPlay();
}

void UCharacterUpgradesComponent::ApplyUpgrade(UCharacterUpgrade* Upgrade)
{
	bool bLevelUpUpgrade = false;
	for (auto kvp : AvailableLevelUpUpgrades)
	{
		for (UCharacterUpgrade* LevelUpUpgrade : kvp.Value.Upgrades)
		{
			if (LevelUpUpgrade == Upgrade)
			{
				bLevelUpUpgrade = true;
				break;
			}
		}
	}

	if (!bLevelUpUpgrade)
		Upgrade = DuplicateObject(Upgrade, GetOwner());

	if (Upgrade->CurrentLevel == 0)
		ActiveUpgrades.Add(Upgrade);

	Upgrade->CurrentLevel++;
	Upgrade->Apply(GetOwner());
}

void UCharacterUpgradesComponent::RemoveUpgrade(UCharacterUpgrade* Upgrade)
{
	Upgrade->Remove(GetOwner());

	ActiveUpgrades.Remove(Upgrade);
}

void UCharacterUpgradesComponent::RequestPerkSelection(WizardElement Element)
{
	OnPerkRequestedDelegate.Broadcast(Element);
}

const FUpgradesArray& UCharacterUpgradesComponent::GetUpgradesArrayForElement(WizardElement Element)
{
	return AvailableLevelUpUpgrades[Element];
}
