// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rooms/RoomTypes.h"
#include "RoomPortal.generated.h"

UCLASS()
class UNREALPROJECT_API ARoomPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomPortal();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void EnablePortal();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DisablePortal();
	UFUNCTION(BlueprintCallable)
	void SetRoomType(RoomType Type);

protected:
	void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class URoomStreamingComponent* RoomStreamingComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInteractableComponent* InteractableComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* PortalTypeWidget;

	UPROPERTY(BlueprintReadWrite);
	RoomType TargetRoomType = RoomType::FireReward;

private:
	UFUNCTION()
	void OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnInteractAction(AActor* Player);
	UFUNCTION()
	void OnRoomLoaded(class ULevelStreaming* LoadedLevel);
	UFUNCTION()
	void OnFadeCompleted();

	UPROPERTY(EditDefaultsOnly)
	FName PlayerTag = TEXT("Player");
	UPROPERTY(Transient)
	AActor* InteractingPlayer;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URoomTypeWidget> PortalTypeWidgetClass;
};
