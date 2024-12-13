// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpellCasting/Elements.h"
#include "ElementTypeWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UElementTypeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetElementType(WizardElement NewElement);

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* TypeImage;

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<WizardElement, UTexture2D*> TypeTextures;
};
