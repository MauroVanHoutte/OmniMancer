#include "PlayerHUD.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include "OmnimancerGameInstance.h"
#include "SpellCasting/ElementManipulationComponent.h"
#include "SpellCasting/Elements.h"
#include "WizardCharacter.h"

void UPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GameInstance)
		SetCurrency(GameInstance->GetCurrency());
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
	GameInstance = GetGameInstance<UOmnimancerGameInstance>();
}
