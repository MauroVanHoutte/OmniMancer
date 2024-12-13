// Fill out your copyright notice in the Description page of Project Settings.

#include "Rooms/RoomStreamingComponent.h"
#include <Engine/LevelStreamingDynamic.h>
#include <Kismet/GameplayStatics.h>

ULevelStreaming* URoomStreamingComponent::LoadNextRoom()
{
	TSoftObjectPtr<UWorld> ChosenLevel = PossibleLevels[0];
	FTransform Transform;
	Transform.SetLocation(GetOwner()->GetActorLocation() + FVector(10000, 0, 0));
	bool bSucces;
	LevelStreamingInstance = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), ChosenLevel, Transform, bSucces);

	if (IsValid(LevelStreamingInstance))
	{
		LevelStreamingInstance->OnLevelShown.AddDynamic(this, &URoomStreamingComponent::OnLevelShown);
	}

	return LevelStreamingInstance;
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
