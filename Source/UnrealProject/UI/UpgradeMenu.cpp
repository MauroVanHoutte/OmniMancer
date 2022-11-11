// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeMenu.h"
#include "../OmnimancerGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h" 
#include "UpgradeButton.h"
#include "UpgradeConnection.h"
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>

void UUpgradeMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetConnectionCanvasOnUpgrades();
	LoadUpgrades();
}

void UUpgradeMenu::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	SetConnectionCanvasOnUpgrades();
}

void UUpgradeMenu::LoadUpgrades()
{
	if (GetGameInstance<UOmnimancerGameInstance>() == nullptr)
		return;
	TSet<FString>& upgrades = GetGameInstance<UOmnimancerGameInstance>()->GetUnlockedUpgrades();

	TArray<UWidget*> widgets = Root->GetAllChildren();
	widgets = widgets.FilterByPredicate([](UWidget* widget) {return widget->IsA<UUpgradeButton>(); });

	for (UWidget* widget : widgets)
	{
		for (FString& upgrade : upgrades)
		{
			UUpgradeButton* upgradeWidget = Cast<UUpgradeButton>(widget);
			if (upgradeWidget->GetName() == upgrade)
			{
				upgradeWidget->SetPreOwned();
				break;
			}
		}
	}
}

void UUpgradeMenu::SetConnectionCanvasOnUpgrades() const
{
	TArray<UWidget*> widgets = Root->GetAllChildren();
	widgets = widgets.FilterByPredicate([](UWidget* widget) {return widget->IsA<UUpgradeButton>(); });

	for (UWidget* widget : widgets)
	{
		Cast<UUpgradeButton>(widget)->SetConnectionCanvas(ConnectionsCanvas);
		Cast<UUpgradeButton>(widget)->SetConnections();
	}
}
