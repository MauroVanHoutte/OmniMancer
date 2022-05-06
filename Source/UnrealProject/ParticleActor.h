// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleActor.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class UNREALPROJECT_API AParticleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AParticleActor();

	void SetSystem(UNiagaraSystem* system, float lifetime);

	void Destroy();
protected:


	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* NiagaraComponent;

	FTimerHandle LifetimeTimer;
};
