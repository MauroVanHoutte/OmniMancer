// Fill out your copyright notice in the Description page of Project Settings.

#include <Components/TextBlock.h>
#include <Components/RichTextBlock.h>
#include "UI/ItemWidget.h"
#include "UI/ItemCostWidget.h"

void UItemWidget::SetName(const FText& ItemName)
{
	if (IsValid(NameText))
		NameText->SetText(ItemName);
}

void UItemWidget::SetDescription(const FText& Description)
{
	if (IsValid(DescriptionText))
		DescriptionText->SetText(Description);
}

void UItemWidget::SetCost(int Cost)
{
	if (IsValid(CostWidget))
		CostWidget->SetCost(Cost);
}
