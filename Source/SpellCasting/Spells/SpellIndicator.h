// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPool/PooledActor.h"
#include "SpellIndicator.generated.h"

UCLASS()
class UNREALPROJECT_API ASpellIndicator : public APooledActor
{
	GENERATED_BODY()
	
public:	
	ASpellIndicator();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartCircleIndicator(const FVector& scale, float lifetime);

protected:
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CylinderMesh;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CylinderProgressMesh;

	USceneComponent* Root;

	float Duration;
	float Timer;
	bool Active = false;
	FVector Scale;

};
