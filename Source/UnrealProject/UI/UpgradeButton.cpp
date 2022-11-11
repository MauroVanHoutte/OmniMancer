// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeButton.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "../OmnimancerGameInstance.h"
#include "UpgradeConnection.h"

bool UUpgradeButton::Initialize()
{
	if (!Super::Initialize())
		return false;

	Name->SetText(FText::FromString(UpgradeName));
	Color->SetColorAndOpacity(UpgradeColor);
	Cost->SetText(FText::FromString(FString::FromInt(UpgradeCost)));
	if (Buyable)
		SetBuyable();
	else if (Bought)
		SetBought();
	else
		SetLocked();

	Button->OnMouseButtonDownEvent.BindUFunction(this, FName("ButtonMouseDown"));

	for (UUpgradeButton* prerequisite : Prerequisites)
	{
		prerequisite->AddDependentUpgrade(this);
	}

	return true;
}

void UUpgradeButton::SynchronizeProperties()
{
	Name->SetText(FText::FromString(UpgradeName));
	Color->SetColorAndOpacity(UpgradeColor);
	Cost->SetText(FText::FromString(FString::FromInt(UpgradeCost)));
}

void UUpgradeButton::SetLocked()
{
	DarkOverlay->SetVisibility(ESlateVisibility::Visible);
	Lock->SetVisibility(ESlateVisibility::Visible);
	Buyable = false;
	Bought = false;
}

void UUpgradeButton::SetBuyable()
{
	DarkOverlay->SetVisibility(ESlateVisibility::Visible);
	Lock->SetVisibility(ESlateVisibility::Hidden);
	Buyable = true;
	Bought = false;
}

void UUpgradeButton::SetBought()
{
	DarkOverlay->SetVisibility(ESlateVisibility::Hidden);
	Lock->SetVisibility(ESlateVisibility::Hidden);
	Buyable = false;
	Bought = true;
}

bool UUpgradeButton::GetBuyable()
{
	return Buyable;
}

bool UUpgradeButton::GetBought()
{
	return Bought;
}

const FString& UUpgradeButton::GetName() const
{
	return UpgradeName;
}

TArray<UUpgradeConnection*>& UUpgradeButton::GetConnectionWidgetsRef()
{
	return ConnectionWidgets;
}

const TArray<UUpgradeButton*>& UUpgradeButton::GetPrerequisites() const
{
	return Prerequisites;
}

void UUpgradeButton::AddDependentUpgrade(UUpgradeButton* upgrade)
{
	Dependents.Add(upgrade);
}

FEventReply UUpgradeButton::ButtonMouseDown(FGeometry& geometry, FPointerEvent mouseEvent)
{
	UOmnimancerGameInstance* gameInstance = GetGameInstance<UOmnimancerGameInstance>();
	if (mouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && Buyable && gameInstance->GetCurrency() > UpgradeCost)
	{
		gameInstance->SetCurrency(gameInstance->GetCurrency() - UpgradeCost);
		AddUpgrade();
		SetBought();
		for (UUpgradeButton* dependentUpgrade : Dependents)
		{
			if (!dependentUpgrade->Bought && dependentUpgrade->ArePrerequisitesBought())
				dependentUpgrade->SetBuyable();
		}
	}

	if (mouseEvent.GetEffectingButton() == EKeys::RightMouseButton && Bought)
	{
		PropagateRefund(true);
	}


	return FEventReply(true);
}

void UUpgradeButton::AddUpgrade()
{
	UOmnimancerGameInstance* gameInstance = GetGameInstance<UOmnimancerGameInstance>();
	gameInstance->AddUnlockedUpgrade(UpgradeName);
	AWizardCharacter* player = GetOwningPlayer<AWizardCharacter>();
	if (player != nullptr)
		player->ApplyUpgrade(UpgradeName);
}

void UUpgradeButton::RemoveUpgrade()
{
	UOmnimancerGameInstance* gameInstance = GetGameInstance<UOmnimancerGameInstance>();
	gameInstance->RemoveUnlockedUpgrade(UpgradeName);
	AWizardCharacter* player = GetOwningPlayer<AWizardCharacter>();
	if (player != nullptr)
		player->RemoveUpgrade(UpgradeName);
}

void UUpgradeButton::PropagateRefund(bool first)
{
	if (!first)
	{
		if (ArePrerequisitesBought())
			SetBuyable();
		else
			SetLocked();
	}
	else
		SetBuyable();

	UOmnimancerGameInstance* gameInstance = GetGameInstance<UOmnimancerGameInstance>();
	gameInstance->SetCurrency(gameInstance->GetCurrency() + UpgradeCost);
	RemoveUpgrade();

	for (UUpgradeButton* dependentUpgrade : Dependents)
	{
		if (dependentUpgrade->Bought)
			dependentUpgrade->PropagateRefund(false);
		else if (!dependentUpgrade->ArePrerequisitesBought())
			dependentUpgrade->SetLocked();
	}
}

void UUpgradeButton::SetPreOwned()
{
	SetBought();
	for (UUpgradeButton* dependentUpgrade : Dependents)
	{
		if (dependentUpgrade->ArePrerequisitesBought() && !Bought)
			dependentUpgrade->SetBuyable();
	}
}

bool UUpgradeButton::ArePrerequisitesBought()
{
	for (UUpgradeButton* prerequisiteUpgrade : Prerequisites)
	{
		if (RequireAllPrerequisites && !prerequisiteUpgrade->Bought)
			return false;
		if (!RequireAllPrerequisites && prerequisiteUpgrade->Bought)
			return true;
	}
	return RequireAllPrerequisites;
}
