// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "PowerUpEffect.h"

#include "PowerUp.generated.h"

UCLASS(Blueprintable)
class UNREALPROJECT_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	

	APowerUp();

	UFUNCTION()
	void OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetRandomEffect();

	virtual void Tick(float DeltaTime) override;

	void LaunchInDirection(const FVector& direction, float strength);

	UFUNCTION(BlueprintCallable)
	void SetEffect(TSubclassOf<UPowerUpEffect> effect);

protected:

	UFUNCTION(CallInEditor)
	void ReInitializeEffects();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* Collider;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPowerUpEffect> Effect;

	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed = 1.f;
	UPROPERTY(EditDefaultsOnly)
	float BobbingAmplitude = 50.f;
	UPROPERTY(EditDefaultsOnly)
	float BobbingSpeed = 1.f;
	float BobbingPhaseOffset = 0.f;

	static TArray<TSubclassOf<UPowerUpEffect>> AllEffects;

	static bool EffectsInitialized;
};
