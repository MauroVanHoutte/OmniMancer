// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/RoomFlowData.h"

void FRoomOptions::GetOptions(TArray<RoomType>& RoomOptions)
{
	RoomOptions.Empty();
	if (!bFloorBoss)
	{
		if (bOptionalBoss)
			RoomOptions.Add(RoomType::Boss);
		if (bHealthGuaranteed)
			RoomOptions.Add(RoomType::HealReward);
		if (bShopGuaranteed)
			RoomOptions.Add(RoomType::Shop);

		int OptionsLeft = TotalOptionsCount - RoomOptions.Num();
		if (OptionsLeft > 0)
		{
			TArray<RoomType> PotentialOptions{RoomType::FireReward, RoomType::IceReward, RoomType::WindReward};
			if (bHealthPossible && !bHealthGuaranteed)
				PotentialOptions.Add(RoomType::HealReward);
			if (bShopPossible && !bShopGuaranteed)
				PotentialOptions.Add(RoomType::Shop);

			for (size_t i = 0; i < OptionsLeft; i++)
			{
				if (PotentialOptions.Num() > 0)
				{
					int RandomOption = FMath::RandRange(0, PotentialOptions.Num() - 1);
					RoomOptions.Add(PotentialOptions[RandomOption]);
					PotentialOptions.RemoveAt(RandomOption);
				}
			}
		}
	}
	else
	{
		RoomOptions.Add(RoomType::FloorBoss);
	}
}

void URoomFlowData::GetRoomOptionsForRoomCount(int RoomCount, TArray<RoomType>& RoomOptions)
{
	if (RoomCount < PredeterminedRoomOptions.Num())
	{
		PredeterminedRoomOptions[RoomCount].GetOptions(RoomOptions);
	}
	else if (!RepeatingOptions.IsEmpty())
	{
		int Rest = (RoomCount - PredeterminedRoomOptions.Num()) % RepeatingOptions.Num();
		RepeatingOptions[Rest].GetOptions(RoomOptions);
	}
}
