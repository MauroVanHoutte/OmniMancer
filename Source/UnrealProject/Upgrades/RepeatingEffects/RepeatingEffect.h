// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../WizardCharacter.h"
#include "../../Spells/FirePool.h"

#include "RepeatingEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract, EditInlineNew)
class UNREALPROJECT_API UBaseRepeatingEffect : public UObject
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter*) {};
	virtual void Remove(AWizardCharacter*) {};
protected:
	FTimerHandle EffectTimer;
	UPROPERTY(EditDefaultsOnly)
	float Interval;
};

UCLASS(BlueprintType, EditInlineNew)
class UNREALPROJECT_API UFireTrail : public UBaseRepeatingEffect
{
	GENERATED_BODY()
public:
	virtual void Apply(AWizardCharacter* wizard) override
	{
		if (FirePools.IsEmpty())
		{
			for (int i = 0; i < NrPools; i++)
			{
				AFirePool* pool = GetWorld()->SpawnActor<AFirePool>(*FirePoolClass);
				pool->InitSpell(wizard->GetActorLocation(), FVector(), wizard);
				pool->SetBurnCause(this); // different pool burns dont stack
				FirePools.Enqueue(pool);
			}
		}
		
		GetWorld()->GetTimerManager().SetTimer(EffectTimer, [wizard, this]()
			{
				if (wizard->IsPendingKill() || !wizard->IsValidLowLevel())
					return;

				AFirePool* pool;
				FirePools.Dequeue(pool);
				pool->InitSpell(wizard->GetActorLocation(), FVector(), wizard);
				FirePools.Enqueue(pool);
			}, Interval, true);
	}

	virtual void Remove(AWizardCharacter* wizard) override
	{
		GetWorld()->GetTimerManager().ClearTimer(EffectTimer);
	}

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFirePool> FirePoolClass;
	int NrPools = 10;
	TQueue<AFirePool*> FirePools;
};
