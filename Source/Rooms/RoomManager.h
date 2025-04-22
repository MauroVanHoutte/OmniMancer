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
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasEncounterStarted();

protected:
	UFUNCTION(BlueprintCallable)
	void OnEncounterCompleted();
	UFUNCTION(BlueprintNativeEvent)
	void StartEnemyWavesEncounter();
	UFUNCTION(BlueprintCallable)
	void StartEncounter();
	UFUNCTION(BlueprintNativeEvent)
	void StartBossEncounter();
	UFUNCTION(BlueprintNativeEvent)
	void OpenShop();

private:
	void SpawnReward();
	void SpawnElementReward();
	void SpawnBossReward();
	void SpawnHealthReward();
	void ActivatePortals();

	UFUNCTION()
	void OnPerkRewardClaimed(AActor* ClaimingPlayer);

	UPROPERTY(EditAnywhere)
	RoomType Type = RoomType::FireReward;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AElementPerkPickup> PerkPickupClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AHealthIncreasePickup> HealthPickupClass;
	bool bHasStarted = false;
};
