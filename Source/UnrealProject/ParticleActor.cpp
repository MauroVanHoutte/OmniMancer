// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleActor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AParticleActor::AParticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("Niagara"));
	RootComponent = NiagaraComponent;
}

void AParticleActor::SetSystem(UNiagaraSystem* system, float lifetime)
{
	NiagaraComponent->SetAsset(system);
	NiagaraComponent->ActivateSystem();
	GetWorld()->GetTimerManager().SetTimer(LifetimeTimer, this, &AParticleActor::Destroy, lifetime);
}

void AParticleActor::Destroy()
{
	AActor::Destroy();
}
