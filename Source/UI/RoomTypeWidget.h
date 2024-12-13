// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rooms/RoomTypes.h"
#include "RoomTypeWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API URoomTypeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetRoomType(RoomType NewType);

protected:
	UPROPERTY(meta=(BindWidget))
	class UImage* TypeImage;

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<RoomType, UTexture2D*> TypeTextures;
};
