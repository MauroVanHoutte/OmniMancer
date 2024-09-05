#include "CharacterUpgrades.h"
#include "Components/WidgetComponent.h"
#include "Health/BaseHealthComponent.h"
#include "Health/DecayingHealthComponent.h"
#include "Health/Healthbar.h"
#include "Health/HealthManager.h"
#include "RepeatingEffects/RepeatingEffect.h"
#include "SpellUpgrades/BaseSpelUpgrade.h"
#include "SpellUpgradesComponent.h"
#include "StatUpgrades/StatBoosts.h"
#include "Triggers/TriggerEffects.h"
#include "TriggerHandlingComponent.h"

bool UCharacterUpgrade::CanBeApplied()
{
	return CurrentLevel < MaxLevel;
}

void USpellUpgrade::Apply(AActor* character)
{
	USpellUpgradesComponent* SpellUpgradesComp = character->GetComponentByClass<USpellUpgradesComponent>();

	UpgradeEffect->CurrentLevel++;

	if (IsValid(SpellUpgradesComp))
	{
		for (const TSubclassOf<ABaseSpell>& SpellType : ApplicableSpells)
		{
			if (CurrentLevel == 1)
			{
				SpellUpgradesComp->RegisterSpellUpgrade(SpellType, UpgradeEffect);
			}

			UpgradeEffect->OnUpgradeRegistered(SpellUpgradesComp, SpellType);
		}
	}
}

void USpellUpgrade::Remove(AActor* character)
{
	USpellUpgradesComponent* SpellUpgradesComp = character->GetComponentByClass<USpellUpgradesComponent>();
	if (IsValid(SpellUpgradesComp))
	{
		for (const TSubclassOf<ABaseSpell>& SpellType : ApplicableSpells)
		{
			SpellUpgradesComp->RemoveSpellUpgrade(SpellType, UpgradeEffect);
		}
	}

	CurrentLevel = 0;
}

FFormatNamedArguments USpellUpgrade::GetDescriptionArguments()
{
	return UpgradeEffect->GetDescriptionArguments();
}

void UTriggerUpgrade::Apply(AActor* character)
{
	UTriggerHandlingComponent* TriggerComponent = character->GetComponentByClass<UTriggerHandlingComponent>();
	if (IsValid(TriggerComponent))
	{
		TriggerComponent->AddTriggerEffect(Condition, TriggerEffect);
	}
}

void UTriggerUpgrade::Remove(AActor* character)
{
	UTriggerHandlingComponent* TriggerComponent = character->GetComponentByClass<UTriggerHandlingComponent>();
	if (IsValid(TriggerComponent))
	{
		TriggerComponent->RemoveTriggerEffect(Condition, TriggerEffect);
	}
}

void UStatUpgrade::Remove(AActor* character)
{

}

void UStatUpgrade::Apply(AActor* character)
{

}

void URepeatingEffectUpgrade::Apply(AActor* character)
{
	Effect->Apply(character);
}

void URepeatingEffectUpgrade::Remove(AActor* character)
{
	Effect->Remove(character);
}

void UShieldUpgrade::Apply(AActor* character)
{
	UHealthManager* HealthManager = character->GetComponentByClass<UHealthManager>();
	if (IsValid(HealthManager) && CurrentLevel == 1)
	{
		if (!IsValid(InstancedComponent))
		{
			InstancedComponent = Cast<UWidgetComponent>(character->AddComponentByClass(HealthbarComponentClass, false, HealthbarComponentClass.GetDefaultObject()->GetRelativeTransform(), false));
			//InstancedComponent->SetWidget(CreateWidget<UUserWidget>(GetWorld(), UserWidget));
		}
		if (!ShieldComponent->HasBegunPlay())
		{
			ShieldComponent->RegisterComponent();
		}

		ShieldComponent->BindHealthbar(InstancedComponent);
		//InstancedComponent->GetUserWidgetObject()->AddToViewport();
		HealthManager->AddHealthComponent(ShieldComponent);
	}

	ShieldComponent->SetMaxHealth(BaseShieldHealth + CurrentLevel * ShieldHealthPerLevel);
}

void UShieldUpgrade::Remove(AActor* character)
{
	UHealthManager* HealthManager = character->GetComponentByClass<UHealthManager>();
	if (IsValid(HealthManager))
	{
		InstancedComponent->GetUserWidgetObject()->RemoveFromParent();
		InstancedComponent->AttachToComponent(nullptr, FAttachmentTransformRules::KeepRelativeTransform);
		character->RemoveInstanceComponent(ShieldComponent);
		HealthManager->RemoveHealthComponent(ShieldComponent);
	}

	CurrentLevel = 0;
}

FFormatNamedArguments UShieldUpgrade::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add("ShieldHealthPerLevel", ShieldHealthPerLevel * CurrentLevel + 1);
	return Args;
}

void UKillBarrierUpgrade::Apply(AActor* character)
{
	Super::Apply(character);

	Trigger->SetHealthComponent(ShieldComponent);
	UTriggerHandlingComponent* TriggerHandling = character->GetComponentByClass<UTriggerHandlingComponent>();
	if (IsValid(TriggerHandling))
	{
		TriggerHandling->AddTriggerEffect(TriggerCondition::OnEnemyKiled, Trigger);
	}
}

void UKillBarrierUpgrade::Remove(AActor* character)
{
	Super::Apply(character);

	UTriggerHandlingComponent* TriggerHandling = character->GetComponentByClass<UTriggerHandlingComponent>();
	if (IsValid(TriggerHandling))
	{
		TriggerHandling->RemoveTriggerEffect(TriggerCondition::OnEnemyKiled, Trigger);
	}
}
