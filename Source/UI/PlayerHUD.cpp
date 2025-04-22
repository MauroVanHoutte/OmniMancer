#include "PlayerHUD.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include "SpellCasting/ElementManipulationComponent.h"
#include "SpellCasting/Elements.h"
#include "Upgrades/StatUpgrades/StatComponent.h"
#include "WizardCharacter.h"

void UPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (StatComponent)
		SetCurrency(StatComponent->GetCurrency());
}

void UPlayerHUD::SetCurrency(int currency)
{
	Currency->SetText(FText::FromString(FString::FromInt(currency)));
}

void UPlayerHUD::SetExperiencePercentage(float expPercentage)
{
	Experience->SetPercent(expPercentage);
}

void UPlayerHUD::Setup()
{
	Player = GetOwningPlayer()->GetPawn();
	SpellCastingComponent = Player->GetComponentByClass<UElementManipulationComponent>();
	StatComponent = Player->GetComponentByClass<UStatComponent>();
}
