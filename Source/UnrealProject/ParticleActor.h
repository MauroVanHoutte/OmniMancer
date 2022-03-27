// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ParticleActor.generated.h"

UCLASS()
class UNREALPROJECT_API AParticleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParticleActor();

	void SetSystem(UNiagaraSystem* system, float lifetime);

	void Destroy();
protected:
	// Called when the game starts or when spawned


	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* m_NiagaraComponent;

	FTimerHandle m_LifetimeTimer;
};
