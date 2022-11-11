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
	virtual void NativeConstruct() override;
	virtual void SynchronizeProperties() override;

private:
	void LoadUpgrades();
	void SetConnectionCanvasOnUpgrades() const;

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
