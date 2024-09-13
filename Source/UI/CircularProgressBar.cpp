// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CircularProgressBar.h"
#include "Components/Image.h"

void UCircularProgressBar::NativeConstruct()
{
	if (IsValid(Progress))
	{
		UMaterialInterface* Material = Cast<UMaterialInterface>(Progress->GetBrush().GetResourceObject());
		if (IsValid(Material))
		{
			ProgressMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Progress->SetBrushFromMaterial(ProgressMaterial);
			SetFillPercentage(0.f);
		}
	}
}

void UCircularProgressBar::SetFillPercentage(float FillPercentage)
{
	if (IsValid(ProgressMaterial))
	{
		ProgressMaterial->SetScalarParameterValue(FName("Fill Percentage"), FillPercentage);
	}
}

void UCircularProgressBar::SetImage(UTexture2D* ImageTexture)
{
	Image->SetBrushFromTexture(ImageTexture);
}
