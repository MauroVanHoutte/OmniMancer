// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoomStreamingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomLoadedSignature, ULevelStreaming*, Level);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API URoomStreamingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
	ULevelStreaming* LoadNextRoom();
	UFUNCTION(BlueprintCallable)
	void UnloadPastRooms();

	UPROPERTY(BlueprintAssignable)
	FOnRoomLoadedSignature OnRoomLoadedDelegate;

private:
	UFUNCTION()
	void OnLevelShown();

	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<UWorld>> PossibleLevels;
	UPROPERTY(Transient)
	class ULevelStreaming* LevelStreamingInstance;
};