// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool/ActorPoolingSubsystem.h"
#include "ActorPool/PooledActorInterface.h"

DEFINE_LOG_CATEGORY(ActorPoolingSubsystem);

void UActorPoolingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	for (auto kvp : StartingActorCount)
	{
		if (IsValid(kvp.Key))
		{
			for (size_t i = 0; i < kvp.Value; i++)
			{
				AddActorOfClassToPool(kvp.Key);
			}
		}
	}
}

void UActorPoolingSubsystem::Deinitialize()
{
	PooledActors.Empty();
}

AActor* UActorPoolingSubsystem::GetActorFromPool(TSubclassOf<AActor> Class)
{
	FPooledActorArray& PooledActorArrayForClass = PooledActors.FindOrAdd(Class);
	FPooledActorStruct* PooledActor = PooledActorArrayForClass.ActorArray.FindByPredicate([](const FPooledActorStruct& Actor)
		{
			return !Actor.bInUse;
		});

	if (PooledActor)
	{
		//PooledActor->PooledActor->ReinitializeProperties(Class.GetDefaultObject());
		PooledActor->bInUse = true;
		if (PooledActor->PooledActor->Implements<UPooledActorInterface>())
		{
			IPooledActorInterface::Execute_OnActorTakenFromPool(PooledActor->PooledActor);
		}
		else
		{
			UE_LOG(ActorPoolingSubsystem, Warning, TEXT("Pooled actor does not implement Interface : %s"), *Class->GetName());
		}
		return PooledActor->PooledActor;
	}
	else
	{
		FPooledActorStruct& NewPooledActor = AddActorOfClassToPool(Class);
		if (IsValid(NewPooledActor.PooledActor))
		{
			NewPooledActor.bInUse = true;
			if (NewPooledActor.PooledActor->Implements<UPooledActorInterface>())
			{
				IPooledActorInterface::Execute_OnActorTakenFromPool(NewPooledActor.PooledActor);
			}
			else
			{
				UE_LOG(ActorPoolingSubsystem, Warning, TEXT("Pooled actor does not implement Interface : %s"), *Class->GetName());
			}
			return NewPooledActor.PooledActor;
		}
		else
		{
			UE_LOG(ActorPoolingSubsystem, Warning, TEXT("Failed to spawn new actor for actor pool"));
			return nullptr;
		}
	}
}

bool UActorPoolingSubsystem::ReturnActorToPool(AActor* Actor)
{
	FPooledActorArray& PooledActorArrayForClass = PooledActors.FindOrAdd(Actor->GetClass());
	FPooledActorStruct* PooledActor = PooledActorArrayForClass.ActorArray.FindByPredicate([Actor](const FPooledActorStruct& CheckedActor)
		{
			return CheckedActor.PooledActor == Actor;
		});

	if (PooledActor)
	{
		PooledActor->bInUse = false;
		if (Actor->Implements<UPooledActorInterface>())
		{
			IPooledActorInterface::Execute_OnActorReturnedToPool(Actor);
		}
		else
		{
			UE_LOG(ActorPoolingSubsystem, Warning, TEXT("Pooled actor does not implement Interface : %s"), *Actor->GetClass()->GetName());
		}
		return true;
	}
	else
	{
		UE_LOG(ActorPoolingSubsystem, Warning, TEXT("Actor %s was returned to pool but is not part of the actor pool"), *Actor->GetClass()->GetName());
		return false;
	}
}

bool UActorPoolingSubsystem::ReturnActorToPoolOrDestroy(AActor* Actor)
{
	bool bReturnedToPool = ReturnActorToPool(Actor);

	if (!bReturnedToPool && IsValid(Actor))
		Actor->Destroy();

	return bReturnedToPool;
}

FPooledActorStruct& UActorPoolingSubsystem::AddActorOfClassToPool(TSubclassOf<AActor> Class)
{
	UE_LOG(ActorPoolingSubsystem, Warning, TEXT("Creating new actor for actor pool of type %s"), *Class->GetName());

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* NewActor = GetWorld()->SpawnActor<AActor>(Class, Params);
	FPooledActorArray& PooledActorArray = PooledActors.FindOrAdd(Class);
	FPooledActorStruct NewPooledActor;
	NewPooledActor.PooledActor = NewActor;
	PooledActorArray.ActorArray.Add(NewPooledActor);
	NewActor->OnEndPlay.AddDynamic(this, &UActorPoolingSubsystem::OnActorEndPlay);
	return PooledActorArray.ActorArray.Last();
}

void UActorPoolingSubsystem::OnActorEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(ActorPoolingSubsystem, Warning, TEXT("Actor %s which is pooled was destroyed"), *Actor->GetActorNameOrLabel());
	FPooledActorArray* ActorArray = PooledActors.Find(Actor->GetClass());
	if (ActorArray)
	{
		ActorArray->ActorArray.RemoveAll([Actor](const FPooledActorStruct& PooledActor)
			{
				return Actor == PooledActor.PooledActor;
			});
	}
}