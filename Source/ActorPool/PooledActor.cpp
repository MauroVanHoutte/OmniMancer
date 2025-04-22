// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool/PooledActor.h"
#include "ActorPool/ActorPoolingSubsystem.h"

bool APooledActor::ReturnToPoolOrDestroy()
{
	if (!IsValid(PoolingSystem))
	{
		PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
	}

	if (IsValid(PoolingSystem))
	{
		return PoolingSystem->ReturnActorToPoolOrDestroy(this);
	}
	else
	{
		Destroy();
		return false;
	}
}

APooledActor* APooledActor::GetActorFromPool(UWorld* World)
{
	if (!IsValid(World))
		return nullptr;

	UActorPoolingSubsystem* ActorPoolingSystem = World->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
	return Cast<APooledActor>(ActorPoolingSystem->GetActorFromPool(StaticClass()));
}

void APooledActor::OnActorTakenFromPool_Implementation()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	EnableComponents();
	OnActorTakenFromPoolDelegate.Broadcast(this);
}

void APooledActor::OnActorReturnedToPool_Implementation()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	DisableComponents();
	OnActorReturnedToPoolDelegate.Broadcast(this);
}

FActorReturnedToPoolSignature& APooledActor::GetReturnedToPoolDelegate()
{
	return OnActorReturnedToPoolDelegate;
}

FActorTakenFromPoolSignature& APooledActor::GetTakenFromPoolDelegate()
{
	return OnActorTakenFromPoolDelegate;
}

void APooledActor::EnableComponents()
{
	TArray<UActorComponent*> Components;
	GetComponents(Components, true);

	for (UActorComponent* Component : Components)
	{
		Component->Activate();
		Component->PrimaryComponentTick.SetTickFunctionEnable(true);
	}
}

void APooledActor::DisableComponents()
{
	TArray<UActorComponent*> Components;
	GetComponents(Components, true);
	
	for (UActorComponent* Component : Components)
	{
		Component->Deactivate();
		Component->PrimaryComponentTick.SetTickFunctionEnable(false);
	}
}
