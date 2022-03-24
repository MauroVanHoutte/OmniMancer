// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "PowerUpEffect.h"

#include "PowerUp.generated.h"

UCLASS()
class UNREALPROJECT_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	

	APowerUp();

	void OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Collider;

	UPROPERTY(EditDefaultsOnly)
	UPowerUpEffect* Effect;
};
