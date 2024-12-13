// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rooms/RoomTypes.h"
#include "RoomManager.generated.h"

UCLASS()
class UNREALPROJECT_API ARoomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomManager();

	void SetRoomType(RoomType NewType);

protected:
	UFUNCTION(BlueprintCallable)
	void OnEncounterCompleted();
	UFUNCTION(BlueprintNativeEvent)
	void StartEnemyWavesEncounter();
	UFUNCTION(BlueprintCallable)
	void StartEncounter();

private:
	void StartBossEncounter();

	void SpawnReward();
	void SpawnElementReward();
	void ActivatePortals();

	UFUNCTION()
	void OnPerkRewardClaimed(AActor* ClaimingPlayer);

	UPROPERTY(VisibleAnywhere)
	RoomType Type;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AElementPerkPickup> PerkPickupClass;
};
