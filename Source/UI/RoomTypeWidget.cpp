// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomTypeWidget.h"
#include <Components/Image.h>

void URoomTypeWidget::SetRoomType(RoomType NewType)
{
	if (IsValid(TypeImage))
	{
		UTexture2D** Texture = TypeTextures.Find(NewType);
		TypeImage->SetBrushFromTexture(Texture ? *Texture : nullptr);
	}
}
