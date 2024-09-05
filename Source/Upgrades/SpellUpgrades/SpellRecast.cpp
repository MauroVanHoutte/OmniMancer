// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellRecast.h"
#include "SpellCasting/Spells/BaseSpell.h"

void USpellRecast::ApplyToSpell(ABaseSpell* Spell)
{
	if (IsValid(Spell))
	{
		RecastCounters.Add(Spell, 0);
		Spell->OnDestroyed.AddUniqueDynamic(this, &USpellRecast::OnSpellDestroyed);
	}
}

FFormatNamedArguments USpellRecast::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add("RecastCount", CurrentLevel + 1);
	Args.Add("SizeMultiplier", SizeMultiplier * 100 - 100);
	Args.Add("DamageMultiplier", 100 - DamageMultiplier * 100);
	return Args;
}

void USpellRecast::OnSpellDestroyed(AActor* DestroyedActor)
{
	ABaseSpell* OriginalSpell = Cast<ABaseSpell>(DestroyedActor);
	int RecastCount = RecastCounters.FindAndRemoveChecked(DestroyedActor);
	RecastCount++;

	ABaseSpell* NewActor = DestroyedActor->GetWorld()->SpawnActor<ABaseSpell>(DestroyedActor->GetClass());

	if (IsValid(OriginalSpell))
	{
		TArray<UBaseSpellUpgradeEffect*> OriginalSpellUpgrades = OriginalSpell->GetAppliedSpellUpgrades();
		UBaseSpellUpgradeEffect** CurrentUpgrade = OriginalSpellUpgrades.FindByPredicate([this](const UBaseSpellUpgradeEffect* Element) {return Element == this; });
		if (CurrentUpgrade)
		{
			OriginalSpellUpgrades.RemoveSingle(*CurrentUpgrade);
		}

		for (UBaseSpellUpgradeEffect* UpgradeData : OriginalSpellUpgrades)
		{
			UpgradeData->ApplyToSpell(NewActor);
			NewActor->TrackAppliedUpgrade(UpgradeData);
		}
	}

	if (RecastCount < CurrentLevel)
	{
		RecastCounters.Add(NewActor, RecastCount);
		NewActor->OnDestroyed.AddUniqueDynamic(this, &USpellRecast::OnSpellDestroyed);
	}

	NewActor->SetScale(OriginalSpell->GetScale() * SizeMultiplier);
	NewActor->SetBaseDamage(OriginalSpell->GetBaseDamage() * DamageMultiplier);
	NewActor->InitSpell(DestroyedActor->GetActorLocation(), DestroyedActor->GetInstigator<APawn>());
}
