// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BaseInteractPickup.h"
#include "ItemPickup.generated.h"

UCLASS()
class UNREALPROJECT_API AItemPickup : public ABaseInteractPickup
{
	GENERATED_BODY()

public:
	AItemPickup();

	UFUNCTION(BlueprintCallable)
	void SetItem(class UItemConfig* NewItem);

protected:
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComponent;

private:
	void OnInteractAction_Implementation(AActor* Player) override;
	UFUNCTION()
	void OnInteractRangeEntered(AActor* Player);
	UFUNCTION()
	void OnInteractRangeExited(AActor* Player);

	UPROPERTY(EditDefaultsOnly)
	int Cost = 0;
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UCharacterUpgrade* ItemUpgrade;
};
