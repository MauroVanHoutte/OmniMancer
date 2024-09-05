// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "PlayerTriggerInterace.h"
#include "Coin.generated.h"


class USphereComponent;
class AWizardCharacter;

UCLASS()
class UNREALPROJECT_API ACoin : public AActor, public IPlayerTriggerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void LaunchInDirection(const FVector& direction, float strength);

	void HomeInToTarget(AActor* Target);

	int GetValue();

	void OnTriggeredByPlayer_Implementation(AActor* Player) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionComponent;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	bool IsExperience = true;
	UPROPERTY(EditAnywhere)
	int Value = 1;
	UPROPERTY(EditAnywhere)
	UCurveFloat* Speed;
	UPROPERTY(VisibleAnywhere)
	float Time = 0.f;
	
	AActor* HomeInTarget;
};
