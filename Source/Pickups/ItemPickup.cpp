// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/ItemPickup.h"
#include <Components/WidgetComponent.h>
#include "Upgrades/CharacterUpgrades.h"
#include "Upgrades/CharacterUpgradesComponent.h"
#include "Upgrades/Items/ItemConfig.h"
#include "Upgrades/StatUpgrades/StatComponent.h"
#include "UI/ItemWidget.h"
#include "Pickups/InteractableComponent.h"

AItemPickup::AItemPickup()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

void AItemPickup::SetItem(UItemConfig* NewItem)
{
	if (IsValid(NewItem))
	{
		ItemUpgrade = NewItem->Upgrade;
		Cost = NewItem->Cost;
		MeshComponent->SetStaticMesh(NewItem->Mesh);
		
		if (IsValid(Widget))
		{
			UItemWidget* ItemWidget = Cast<UItemWidget>(Widget->GetUserWidgetObject());
			if (IsValid(ItemWidget))
			{
				ItemWidget->SetName(ItemUpgrade->Name);
				ItemWidget->SetDescription(ItemUpgrade->GetFormattedDescription());
				ItemWidget->SetCost(Cost);
			}
		}
	}
}

void AItemPickup::BeginPlay()
{
	Super::BeginPlay();

	/*if (IsValid(Widget))
	{
		if (Cost <= 0)
		{
			Widget->SetVisibility(false);
		}
		else
		{
			UItemWidget* ItemWidget = Cast<UItemWidget>(Widget->GetUserWidgetObject());
			if (IsValid(ItemWidget))
			{
				ItemWidget->SetCost(Cost);
			}
		}
	}*/

	InteractactableComponent->OnInteractRangeEnteredDelegate.AddDynamic(this, &AItemPickup::OnInteractRangeEntered);
	InteractactableComponent->OnInteractRangeExitedDelegate.AddDynamic(this, &AItemPickup::OnInteractRangeExited);
}

void AItemPickup::OnInteractAction_Implementation(AActor* Player)
{
	if (IsValid(ItemUpgrade))
	{
		UStatComponent* Stats = Player->GetComponentByClass<UStatComponent>();
		if ((IsValid(Stats) && Stats->GetCurrency() > Cost)
			|| Cost <= 0)
		{
			if (IsValid(Stats))
				Stats->SetCurrency(Stats->GetCurrency() - Cost);
			UCharacterUpgradesComponent* UpgradesComponent = Player->GetComponentByClass<UCharacterUpgradesComponent>();
			if (IsValid(UpgradesComponent))
				UpgradesComponent->ApplyUpgrade(ItemUpgrade);
			else
				ItemUpgrade->Apply(Player);

			ReturnToPoolOrDestroy();
		}
	}
}

void AItemPickup::OnInteractRangeEntered(AActor* Player)
{
	if (IsValid(Widget))
	{
		UItemWidget* ItemWidget = Cast<UItemWidget>(Widget->GetUserWidgetObject());
		if (IsValid(ItemWidget))
		{
			ItemWidget->ShowDescription();
		}
	}
}

void AItemPickup::OnInteractRangeExited(AActor* Player)
{
	if (IsValid(Widget))
	{
		UItemWidget* ItemWidget = Cast<UItemWidget>(Widget->GetUserWidgetObject());
		if (IsValid(ItemWidget))
		{
			ItemWidget->HideDescription();
		}
	}
}
