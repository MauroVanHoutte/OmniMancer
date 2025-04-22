// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ItemCostWidget.h"
#include <Components/TextBlock.h>
#include "Upgrades/StatUpgrades/StatComponent.h"

void UItemCostWidget::SetCost(int NewCost)
{
	Cost = NewCost;
	ItemCost->SetText(FText::FromString(FString::FromInt(Cost)));
}

void UItemCostWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!IsValid(PlayerStatComponent))
	{
		APlayerController* Controller = GetWorld()->GetFirstPlayerController();
		if (IsValid(Controller))
		{
			APawn* Pawn = Controller->GetPawn();
			if (IsValid(Pawn))
			{
				PlayerStatComponent = Pawn->GetComponentByClass<UStatComponent>();
			}
		}
	}

	if (IsValid(PlayerStatComponent))
	{
		int Curreny = PlayerStatComponent->GetCurrency();
		if (Curreny >= Cost)
		{
			ItemCost->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		}
		else
		{
			ItemCost->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		}
	}
}
