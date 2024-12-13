// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementTypeWidget.h"
#include <Components/Image.h>

void UElementTypeWidget::SetElementType(WizardElement NewElement)
{
	if (IsValid(TypeImage))
	{
		UTexture2D** Texture = TypeTextures.Find(NewElement);
		TypeImage->SetBrushFromTexture(Texture ? *Texture : nullptr);
	}
}
