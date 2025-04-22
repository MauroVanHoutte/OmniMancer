// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemList.generated.h"

UCLASS(BlueprintType)
class UNREALPROJECT_API UItemList : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<class UItemConfig*> Items;
};
