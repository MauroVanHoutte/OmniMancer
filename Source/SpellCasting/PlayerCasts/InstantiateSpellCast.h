// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellCasting/PlayerCasts/BasePlayerCast.h"
#include "InstantiateSpellCast.generated.h"

UCLASS()
class UNREALPROJECT_API UInstantiateSpellCast : public UBasePlayerCast
{
	GENERATED_BODY()

public:
	void HandleCastInputPressed_Implementation() override;
	void HandleCastInputDown_Implementation() override;
	void HandleCastInputReleased_Implementation() override;

private:
	UFUNCTION()
	void CastSpell();
	float CalculateCooldown();
	UFUNCTION()
	void OnSpellHit(class ABaseSpell* Spell, class AActor* HitActor);
	FVector GetMousePosAtWizardHeight();

	UPROPERTY(EditDefaultsOnly)
	bool bRepeatCasting = false;
	UPROPERTY(EditDefaultsOnly)
	float CastTime = 0.1f;
	FTimerHandle CastTimer;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseSpell> SpellType;
};

