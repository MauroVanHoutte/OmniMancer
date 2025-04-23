// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeButton.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "OmnimancerGameInstance.h"
#include "UpgradeConnection.h"
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>

void UUpgradeButton::NativeConstruct()
{
	Super::NativeConstruct();

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

	SetConnections();
}

void UUpgradeButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	Name->SetText(FText::FromString(UpgradeName));
	Color->SetColorAndOpacity(UpgradeColor);
	Cost->SetText(FText::FromString(FString::FromInt(UpgradeCost)));

	if (Buyable)
		SetBuyable();
	else if (Bought)
		SetBought();
	else
		SetLocked();

	if (ConnectionType == nullptr)
		return;

	int difference = Prerequisites.Num() - ConnectionWidgets.Num();
	while (difference != 0)
	{
		int sign = FMath::Sign(difference);
	
		if (sign > 0)
		{
			ConnectionWidgets.Add(CreateWidget<UUpgradeConnection>(WidgetTree, ConnectionType, FName()));
		}
		else
		{
			ConnectionWidgets[ConnectionWidgets.Num() - 1]->RemoveFromParent();
			ConnectionWidgets.RemoveAt(ConnectionWidgets.Num() - 1);
		}
		difference += sign * -1;
	}
}

void UUpgradeButton::SetLocked()
{
	DarkOverlay->SetVisibility(ESlateVisibility::Visible);
	Lock->SetVisibility(ESlateVisibility::Visible);
	Buyable = false;
	Bought = false;

	for (UUpgradeConnection* connection : ConnectionWidgets)
	{
		connection->SetCoreColor(LockedColor);
	}
}

void UUpgradeButton::SetBuyable()
{
	DarkOverlay->SetVisibility(ESlateVisibility::Visible);
	Lock->SetVisibility(ESlateVisibility::Hidden);
	Buyable = true;
	Bought = false;

	for (size_t i = 0; i < Prerequisites.Num(); i++)
	{
		if (Prerequisites[i]->Bought)
			ConnectionWidgets[i]->SetCoreColor(BuyableColor);
	}
}

void UUpgradeButton::SetBought()
{
	DarkOverlay->SetVisibility(ESlateVisibility::Hidden);
	Lock->SetVisibility(ESlateVisibility::Hidden);
	Buyable = false;
	Bought = true;

	for (UUpgradeConnection* connection : ConnectionWidgets)
	{
		connection->SetCoreColor(BoughtColor);
	}
}

bool UUpgradeButton::GetBuyable() const
{
	return Buyable;
}

bool UUpgradeButton::GetBought() const
{
	return Bought;
}

void UUpgradeButton::SetConnectionCanvas(UCanvasPanel* canvas)
{
	ConnectionCanvas = canvas;
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
	/*AWizardCharacter* player = GetOwningPlayer<AWizardCharacter>();
	if (player != nullptr)
		player->ApplyUpgrade(UpgradeName);*/
}

void UUpgradeButton::RemoveUpgrade()
{
	UOmnimancerGameInstance* gameInstance = GetGameInstance<UOmnimancerGameInstance>();
	gameInstance->RemoveUnlockedUpgrade(UpgradeName);
	//AWizardCharacter* player = GetOwningPlayer<AWizardCharacter>();
	//if (player != nullptr)
	//	player->RemoveUpgrade(UpgradeName);
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
		if (dependentUpgrade->ArePrerequisitesBought() && !dependentUpgrade->Bought)
			dependentUpgrade->SetBuyable();
	}
}

int32 UUpgradeButton::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	//Upgrade buttons dont move during play so updating the connections is not needed
	if (GetWorld()->IsGameWorld())
		return 0;

	//SetConnections();

	return 0;
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

void UUpgradeButton::SetConnections() const
{
	if (ConnectionCanvas == nullptr)
		return;

	UCanvasPanelSlot* upgradeSlot = Cast<UCanvasPanelSlot>(Slot);

	for (size_t i = 0; i < Prerequisites.Num(); i++)
	{
		if (IsValid(Prerequisites[i]))
		{
			Cast<UCanvasPanelSlot>(ConnectionCanvas->AddChild(ConnectionWidgets[ConnectionWidgets.Num() - 1]))->SetAlignment(FVector2D(0.5f, 0.5f));

			UCanvasPanelSlot* prereqSlot = Cast<UCanvasPanelSlot>(Prerequisites[i]->Slot);
			UCanvasPanelSlot* connectionSlot = Cast<UCanvasPanelSlot>(ConnectionWidgets[i]->Slot);

			FVector2D prereqPos = prereqSlot->GetPosition();
			FVector2D upgradePos = upgradeSlot->GetPosition();

			connectionSlot->SetPosition((prereqPos + upgradePos) / 2.f);
			connectionSlot->SetSize(FVector2D(20.f, FVector2D::Distance(prereqPos, upgradePos)));

			FVector2D direction = prereqPos - upgradePos;
			direction.Normalize();

			float angle = FMath::RadiansToDegrees(FMath::Atan2(direction.Y, direction.X)) - 90;

			ConnectionWidgets[i]->SetRenderTransformAngle(angle);
			//Set color
			if (Bought)
				ConnectionWidgets[i]->SetCoreColor(BoughtColor);
			else if (Prerequisites[i]->Bought)
				ConnectionWidgets[i]->SetCoreColor(BuyableColor);
			else
				ConnectionWidgets[i]->SetCoreColor(LockedColor);
		}
	}
}
