// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AWizardCharacter;

namespace TriggerEffects
{
	class BaseTriggerEffect
	{
	public:

		virtual void OnTrigger(AWizardCharacter* caster, AActor* target) {};
	};

	//Slow enemies in radius around caster
	class AoeSlowTrigger : public BaseTriggerEffect
	{
	public:
		void OnTrigger(AWizardCharacter* caster, AActor* target) override
		{
			TArray<AActor*> ignore{ caster };
			TArray<AActor*> outActors;
			UKismetSystemLibrary::SphereOverlapActors(caster->GetWorld(), caster->GetActorLocation(), Radius, TArray<TEnumAsByte<EObjectTypeQuery>>(), ABaseCharacter::StaticClass(), ignore, outActors);
			for (AActor* actor : outActors)
			{
				auto enemy = Cast<ABaseCharacter>(actor);
				if (enemy->IsValidLowLevel())
					enemy->AddStatusEffect(FStatusEffect{ Type::Slow, -1, Slow , Duration, caster });
			}
		};

		void SetVars(float radius, float slow, float duration)
		{
			Radius = radius;
			Slow = slow;
			Duration = duration;
		};

	private:
		float Radius = 600;
		float Slow = 20;
		float Duration = 3;
	};

	//Give the caster a speed boost
	class SpeedBuffTrigger : public BaseTriggerEffect
	{
	public:
		void OnTrigger(AWizardCharacter* caster, AActor* target) override
		{
			caster->AddStatusEffect(FStatusEffect{ Type::Slow, -1, -Speed, Duration, caster });
		};

		void SetVars(float speed, float duration)
		{
			Speed = speed;
			Duration = duration;
		};

	private:
		float Speed = 20;
		float Duration = 3;
	};

	//Chance to lower Cooldowns
	class LowerCooldownTrigger : public BaseTriggerEffect
	{
	public:
		void OnTrigger(AWizardCharacter* caster, AActor* target) override
		{
			int rand = FMath::RandRange(0, 100);
			if (rand < Chance)
				caster->LowerCooldowns(Amount);
		};

		void SetVars(int chance, float amount)
		{
			Chance = chance;
			Amount = amount;
		};

	private:
		int Chance = 30;
		float Amount = 1;
	};
}

