// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>

#include "CloudActor.generated.h"

UCLASS()
class UNREALPROJECT_API ACloudActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACloudActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void CheckOverlapBegin();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CloudMesh;
	UMaterialInstanceDynamic* CloudMatInstance;
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* CollisionComponent;
	bool IsRising = false;
	UPROPERTY(EditDefaultsOnly)
	float RisePerSecond = 500.f;
	UPROPERTY(EditDefaultsOnly)
	float ShrinkPerSecond = 1.f;
};
