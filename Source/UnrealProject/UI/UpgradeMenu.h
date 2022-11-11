// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeMenu.generated.h"

class UCanvasPanel;
class UUpgradeConnection;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UUpgradeMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

protected:
	int32 NativePaint(const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;

private:
	void LoadUpgrades();

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* ConnectionsCanvas;
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Root;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUpgradeConnection> ConnectionType;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	FLinearColor BoughtColor;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	FLinearColor BuyableColor;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	FLinearColor LockedColor;
};
