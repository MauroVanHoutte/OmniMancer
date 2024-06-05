// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellCasting/SpellUpgrades/FlameColumnRecast.h"
#include "SpellCasting/Spells/BaseSpell.h"
#include "SpellCasting/Spells/FlameColumn.h"

void UFlameColumnRecast::ApplyToSpell(ABaseSpell* Spell)
{
	if (IsValid(Spell))
	{
		Spell->OnDestroyed.AddDynamic(this, &UFlameColumnRecast::OnFlameColumnDestroyed);
	}
}

void UFlameColumnRecast::OnFlameColumnDestroyed(AActor* DestroyedActor)
{
	AFlameColumn* NewActor = DestroyedActor->GetWorld()->SpawnActor<AFlameColumn>(DestroyedActor->GetClass());
	NewActor->InitSpell(DestroyedActor->GetActorLocation(), DestroyedActor->GetInstigator<APawn>());
	NewActor->ReinitializeProperties(DestroyedActor);
}
