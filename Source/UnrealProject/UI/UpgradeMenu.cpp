// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeMenu.h"
#include "../OmnimancerGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h" 
#include "UpgradeButton.h"
#include "UpgradeConnection.h"
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>

bool UUpgradeMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	LoadUpgrades();
	return true;
}

int32 UUpgradeMenu::NativePaint(const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	TArray<UWidget*> widgets = Root->GetAllChildren();
	widgets = widgets.FilterByPredicate([](UWidget* widget) {return widget->IsA<UUpgradeButton>(); });


	for (UWidget* widget : widgets)
	{
		UUpgradeButton* upgradeWidget = Cast<UUpgradeButton>(widget);
		
		const TArray<UUpgradeButton*>& prerequisites = upgradeWidget->GetPrerequisites();
		TArray<UUpgradeConnection*>& connectionWidgets = upgradeWidget->GetConnectionWidgetsRef();

		//Create connection widgets
		if (prerequisites.Num() != connectionWidgets.Num())
		{
			connectionWidgets.Empty(prerequisites.Num());

			for (size_t i = 0; i < prerequisites.Num(); i++)
			{
				UUpgradeConnection* connection = CreateWidget<UUpgradeConnection>(GetOwningPlayer(), ConnectionType);
				if (connection == nullptr)
					return 0;
				Cast<UCanvasPanelSlot>(ConnectionsCanvas->AddChild(connection))->SetAlignment(FVector2D(0.5f, 0.5f));
				connectionWidgets.Add(connection);
			}
		}

		UCanvasPanelSlot* upgradeSlot = Cast<UCanvasPanelSlot>(upgradeWidget->Slot);
		//Set position and rotation of connections
		for (size_t i = 0; i < prerequisites.Num(); i++)
		{
			UCanvasPanelSlot* prereqSlot = Cast<UCanvasPanelSlot>(prerequisites[i]->Slot);
			UCanvasPanelSlot* connectionSlot = Cast<UCanvasPanelSlot>(connectionWidgets[i]->Slot);

			FVector2D prereqPos = prereqSlot->GetPosition();
			FVector2D upgradePos = upgradeSlot->GetPosition();

			connectionSlot->SetPosition((prereqPos + upgradePos) / 2.f);
			connectionSlot->SetSize(FVector2D(20.f, FVector2D::Distance(prereqPos, upgradePos)));

			FVector2D direction = prereqPos - upgradePos;

			connectionWidgets[i]->SetRenderTransformAngle(FMath::Atan2(direction.Y, direction.X));

			//Set color
			if (upgradeWidget->GetBought())
				connectionWidgets[i]->SetCoreColor(BoughtColor);
			else if (prerequisites[i]->GetBought())
				connectionWidgets[i]->SetCoreColor(BuyableColor);
			else
				connectionWidgets[i]->SetCoreColor(LockedColor);
		}
	}
	return 0;
}

void UUpgradeMenu::LoadUpgrades()
{
	if (GetGameInstance<UOmnimancerGameInstance>() == nullptr)
		return;
	TSet<FString>& upgrades = GetGameInstance<UOmnimancerGameInstance>()->GetUnlockedUpgrades();

	TArray<UWidget*> widgets = Root->GetAllChildren();
	widgets = widgets.FilterByPredicate([](UWidget* widget) {return widget->IsA<UUpgradeButton>(); });

	for (FString& upgrade : upgrades)
	{
		for (UWidget* widget : widgets)
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
