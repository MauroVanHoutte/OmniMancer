// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellCasting/SpellUpgrades/FlameColumnRecast.h"
#include "SpellCasting/Spells/BaseSpell.h"
#include "SpellCasting/Spells/FlameColumn.h"

void UFlameColumnRecast::ApplyToSpell(ABaseSpell* Spell)
{
	if (IsValid(Spell))
	{
		AFlameColumn* NewActor = Spell->GetWorld()->SpawnActor<AFlameColumn>(Spell->GetClass());
		NewActor->SetActorHiddenInGame(true);
		NewActor->SetActorTickEnabled(false);
		OriginalToRecast.Add(Spell, NewActor);

		Spell->OnDestroyed.AddUniqueDynamic(this, &UFlameColumnRecast::OnFlameColumnDestroyed);
	}
}

void UFlameColumnRecast::OnFlameColumnDestroyed(AActor* DestroyedActor)
{
	ABaseSpell* OriginalSpell = Cast<ABaseSpell>(DestroyedActor);
	ABaseSpell* RecastSpell = Cast<ABaseSpell>(OriginalToRecast.FindAndRemoveChecked(DestroyedActor));
	if (IsValid(RecastSpell))
	{
		if (IsValid(OriginalSpell))
		{
			TArray<USpellUpgradeData*> OriginalSpellUpgrades = OriginalSpell->GetAppliedSpellUpgrades();
			USpellUpgradeData** CurrentUpgrade = OriginalSpellUpgrades.FindByPredicate([this](const USpellUpgradeData* Element) {return Element->Upgrade == this; });
			if (CurrentUpgrade)
			{
				OriginalSpellUpgrades.RemoveSingle(*CurrentUpgrade);
			}

			for (USpellUpgradeData* UpgradeData : OriginalSpellUpgrades)
			{
				UpgradeData->Upgrade->ApplyToSpell(RecastSpell);
				RecastSpell->TrackAppliedUpgrade(UpgradeData);
			}
		}

		RecastSpell->SetActorHiddenInGame(false);
		RecastSpell->SetActorTickEnabled(true);
		RecastSpell->SetScale(OriginalSpell->GetScale() * SizeMultiplier);
		RecastSpell->SetBaseDamage(OriginalSpell->GetBaseDamage() * DamageMultiplier);
		RecastSpell->InitSpell(DestroyedActor->GetActorLocation(), DestroyedActor->GetInstigator<APawn>());
	}
	//NewActor->ReinitializeProperties(DestroyedActor);
}
