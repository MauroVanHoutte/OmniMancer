// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Rooms/RoomTypes.h"
#include "RoomFlowData.generated.h"

USTRUCT(BlueprintType)
struct FRoomOptions
{
	GENERATED_BODY()

	void GetOptions(TArray<RoomType>& RoomOptions);

	UPROPERTY(EditDefaultsOnly)
	bool bFloorBoss = false;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!bFloorBoss"))
	int TotalOptionsCount = 3;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!bFloorBoss"))
	bool bOptionalBoss = false;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!bFloorBoss"))
	bool bHealthPossible = false;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bHealthPossible && !bFloorBoss"))
	bool bHealthGuaranteed = false;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!bFloorBoss"))
	bool bShopPossible = false;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bShopPossible && !bFloorBoss"))
	bool bShopGuaranteed = false;
};

UCLASS()
class UNREALPROJECT_API URoomFlowData : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void GetRoomOptionsForRoomCount(int RoomCount, TArray<RoomType>& RoomOptions);

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FRoomOptions> PredeterminedRoomOptions;
	UPROPERTY(EditDefaultsOnly)
	TArray<FRoomOptions> RepeatingOptions;
};
