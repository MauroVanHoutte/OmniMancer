// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleActor.h"

// Sets default values
AParticleActor::AParticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("Niagara"));
	RootComponent = m_NiagaraComponent;
}

void AParticleActor::SetSystem(UNiagaraSystem* system, float lifetime)
{
	m_NiagaraComponent->SetAsset(system);
	m_NiagaraComponent->ActivateSystem();
	GetWorld()->GetTimerManager().SetTimer(m_LifetimeTimer, this, &AParticleActor::Destroy, lifetime);
}

void AParticleActor::Destroy()
{
	AActor::Destroy();
}
