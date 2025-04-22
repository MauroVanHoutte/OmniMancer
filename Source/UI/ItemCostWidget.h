// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemCostWidget.generated.h"

UCLASS()
class UNREALPROJECT_API UItemCostWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetCost(int NewCost);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	int Cost = 0;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ItemCost;
	class UStatComponent* PlayerStatComponent;
};
