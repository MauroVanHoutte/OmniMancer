// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ElementalAltarUi.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExitButtonClickedSignature, UButton*, Button);

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UElementalAltarUi : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetPlayerActor(AActor* PlayerActor);
	void SetPortal(class ARoomPortal* RoomPortal);

	UPROPERTY(meta = (BindWidget))
	class UButton* FireButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* IceButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* WindButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(BlueprintAssignable)
	FExitButtonClickedSignature ExitButtonClickedDelegate;

private:
	UFUNCTION()
	void FireButtonClicked();
	UFUNCTION()
	void IceButtonClicked();
	UFUNCTION()
	void WindButtonClicked();
	UFUNCTION()
	void ExitButtonClicked();

	UPROPERTY(Transient)
	class ARoomPortal* Portal;
	UPROPERTY(Transient)
	class UElementManipulationComponent* InteractingPlayerSpellcasting;
};
