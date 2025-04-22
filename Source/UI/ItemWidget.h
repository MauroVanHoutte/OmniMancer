// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

UCLASS()
class UNREALPROJECT_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowDescription();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void HideDescription();
	void SetName(const FText& ItemName);
	void SetDescription(const FText& Description);
	void SetCost(int Cost);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* NameText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class URichTextBlock* DescriptionText;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UItemCostWidget* CostWidget;
};
