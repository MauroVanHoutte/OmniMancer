// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeButton.generated.h"

class UUpgradeConnection;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UUpgradeButton : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void SynchronizeProperties() override;

	void SetLocked();
	void SetBuyable();
	void SetBought();
	bool GetBuyable() const;
	bool GetBought() const;

	void SetConnectionCanvas(UCanvasPanel* canvas);
	void SetConnections() const;

	const FString& GetName() const;
	TArray<UUpgradeConnection*>& GetConnectionWidgetsRef();
	const TArray<UUpgradeButton*>& GetPrerequisites() const;
	void AddDependentUpgrade(UUpgradeButton* upgrade);

	UFUNCTION(BlueprintCallable)
	void SetPreOwned();

protected:
	int32 NativePaint(const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;
private:
	UFUNCTION()
	FEventReply ButtonMouseDown(FGeometry& geometry, FPointerEvent mouseEvent);

	void AddUpgrade();
	void RemoveUpgrade();
	void PropagateRefund(bool first);

	bool ArePrerequisitesBought();

	UPROPERTY(EditAnywhere)
	FString UpgradeName = "";
	UPROPERTY(EditAnywhere)
	int UpgradeCost = 0;
	UPROPERTY(EditAnywhere)
	FLinearColor UpgradeColor;
	UPROPERTY(EditAnywhere)
	bool RequireAllPrerequisites = false;
	UPROPERTY(EditAnywhere)
	bool Buyable = false;
	UPROPERTY(EditAnywhere)
	bool Bought = false;
	UPROPERTY(EditAnywhere)
	TArray<UUpgradeButton*> Prerequisites;
	TArray<UUpgradeButton*> Dependents;
	TArray<UUpgradeConnection*> ConnectionWidgets;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUpgradeConnection> ConnectionType;
	UCanvasPanel* ConnectionCanvas;
	UPROPERTY(EditAnywhere)
	FLinearColor BoughtColor;
	UPROPERTY(EditAnywhere)
	FLinearColor BuyableColor;
	UPROPERTY(EditAnywhere)
	FLinearColor LockedColor;

	UPROPERTY( meta = (BindWidget))
	class UTextBlock* Name;
	UPROPERTY( meta = (BindWidget))
	class UTextBlock* Cost;
	UPROPERTY( meta = (BindWidget))
	class UImage* Button;
	UPROPERTY( meta = (BindWidget))
	class UImage* Color;
	UPROPERTY( meta = (BindWidget))
	class UImage* DarkOverlay;
	UPROPERTY( meta = (BindWidget))
	class UImage* Lock;
};
