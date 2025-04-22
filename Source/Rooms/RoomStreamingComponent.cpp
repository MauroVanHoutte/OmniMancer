// Fill out your copyright notice in the Description page of Project Settings.

#include "Rooms/RoomStreamingComponent.h"
#include <Engine/LevelStreamingDynamic.h>
#include <Kismet/GameplayStatics.h>
#include "Rooms/RoomTypeLevels.h"

ULevelStreaming* URoomStreamingComponent::LoadNextRoom(RoomType Type)
{
	if (IsValid(Levels) && Levels->LevelArrays.Contains(Type))
	{
		FLevelArray* LevelsForType = Levels->LevelArrays.Find(Type);
		if (!LevelsForType->LevelArray.IsEmpty())
		{
			FTransform Transform;
			Transform.SetLocation(GetOwner()->GetActorLocation() + FVector(10000, 0, 0));
			GEngine->AddOnScreenDebugMessage(1213, 10, FColor::Red, Transform.GetLocation().ToString());
			int NrOptions = LevelsForType->LevelArray.Num();
			TSoftObjectPtr<UWorld> ChosenLevel = LevelsForType->LevelArray[FMath::RandRange(0, NrOptions - 1)];
			bool bSucces;
			LevelStreamingInstance = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), ChosenLevel, Transform, bSucces);

			if (IsValid(LevelStreamingInstance))
			{
				LevelStreamingInstance->OnLevelShown.AddDynamic(this, &URoomStreamingComponent::OnLevelShown);
			}

			return LevelStreamingInstance;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RoomTypeLevels asset does not contain any levels for type"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RoomTypeLevels asset invalid or does not contain any levels for type"));
	}

	return nullptr;
}

void URoomStreamingComponent::UnloadPastRooms()
{
	ULevel* OwnerLevel = GetOwner()->GetLevel();
	TArray<ULevelStreaming*> StreamingLevels = GetWorld()->GetStreamingLevels();
	for (ULevelStreaming* StreamingLevel : StreamingLevels)
	{
		ULevel* Level = StreamingLevel->GetLoadedLevel();
		if (OwnerLevel != Level)
		{
			StreamingLevel->SetIsRequestingUnloadAndRemoval(true);
			if (IsValid(Level))
			{
				UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(), Level, FLatentActionInfo(), false);
			}
		}
	}
}

void URoomStreamingComponent::OnLevelShown()
{
	OnRoomLoadedDelegate.Broadcast(LevelStreamingInstance);
}
