// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetKeyText(const FText& NewText);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyText;
};
