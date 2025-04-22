// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Rooms/RoomTypes.h"
#include "RoomTypeLevels.generated.h"

USTRUCT(BlueprintType)
struct UNREALPROJECT_API FLevelArray
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<class UWorld>> LevelArray;
};

UCLASS()
class UNREALPROJECT_API URoomTypeLevels : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TMap<RoomType, FLevelArray> LevelArrays;
};
