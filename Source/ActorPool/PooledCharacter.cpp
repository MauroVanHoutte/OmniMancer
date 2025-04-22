// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool/PooledCharacter.h"
#include "ActorPool/ActorPoolingSubsystem.h"

bool APooledCharacter::ReturnToPoolOrDestroy()
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

APooledCharacter* APooledCharacter::GetActorFromPool(UWorld* World)
{
	if (!IsValid(World))
		return nullptr;

	UActorPoolingSubsystem* ActorPoolingSystem = World->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
	return Cast<APooledCharacter>(ActorPoolingSystem->GetActorFromPool(StaticClass()));
}

void APooledCharacter::OnActorTakenFromPool_Implementation()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	EnableComponents();
	OnActorTakenFromPoolDelegate.Broadcast(this);
}

void APooledCharacter::OnActorReturnedToPool_Implementation()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	DisableComponents();
	OnActorReturnedToPoolDelegate.Broadcast(this);
}

FActorReturnedToPoolSignature& APooledCharacter::GetReturnedToPoolDelegate()
{
	return OnActorReturnedToPoolDelegate;
}

FActorTakenFromPoolSignature& APooledCharacter::GetTakenFromPoolDelegate()
{
	return OnActorTakenFromPoolDelegate;
}

void APooledCharacter::EnableComponents()
{
	TArray<UActorComponent*> Components;
	GetComponents(Components, true);

	for (UActorComponent* Component : Components)
	{
		Component->Activate();
		Component->PrimaryComponentTick.SetTickFunctionEnable(true);
	}
}

void APooledCharacter::DisableComponents()
{
	TArray<UActorComponent*> Components;
	GetComponents(Components, true);

	for (UActorComponent* Component : Components)
	{
		Component->Deactivate();
		Component->PrimaryComponentTick.SetTickFunctionEnable(false);
	}
}
