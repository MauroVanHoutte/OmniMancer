#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class RoomType : uint8
{
	FireReward,
	IceReward,
	WindReward,
	HealReward,
	Boss,
	Shop,
	FloorBoss
};