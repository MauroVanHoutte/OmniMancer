// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeConnection.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UUpgradeConnection : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetCoreColor(FLinearColor color);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Core;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Sides;
};
