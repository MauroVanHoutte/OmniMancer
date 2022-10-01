#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include "PlayerHUD.h"
#include "../OmnimancerGameInstance.h"
#include "../WizardCharacter.h"

void UPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	auto gameInstance = GetGameInstance<UOmnimancerGameInstance>();
	if (gameInstance)
		SetCurrency(gameInstance->GetCurrency());

	auto player = GetOwningPlayer()->GetPawn<AWizardCharacter>();
	if (player)
	{
		SetCooldowns(player->GetCooldowns(), player->GetCooldownTimers());
		SetExperiencePercentage(player->GetExperiencePercentage());
	}
}

void UPlayerHUD::SetCurrency(int currency)
{
	Currency->SetText(FText::FromString(FString::FromInt(currency)));
}

void UPlayerHUD::SetExperiencePercentage(float expPercentage)
{
	Experience->SetPercent(expPercentage);
}

void UPlayerHUD::SetCooldowns(const TMap<int, float>& Cooldowns, const TMap<int, FTimerHandle>& Timers)
{
	for (auto& it : Cooldowns)
	{
		switch (it.Key)
		{
		case 0:
			Cooldown0->GetDynamicMaterial()->SetScalarParameterValue(FName("Fill Percentage"), GetWorld()->GetTimerManager().GetTimerRemaining(Timers[0])/it.Value);
			break;
		case 1:
			Cooldown1->GetDynamicMaterial()->SetScalarParameterValue(FName("Fill Percentage"), GetWorld()->GetTimerManager().GetTimerRemaining(Timers[1]) / it.Value);
			break;
		case 2:
			Cooldown2->GetDynamicMaterial()->SetScalarParameterValue(FName("Fill Percentage"), GetWorld()->GetTimerManager().GetTimerRemaining(Timers[2]) / it.Value);
			break;
		case 3:
			Cooldown3->GetDynamicMaterial()->SetScalarParameterValue(FName("Fill Percentage"), GetWorld()->GetTimerManager().GetTimerRemaining(Timers[3]) / it.Value);
			break;
		case 4:
			Cooldown4->GetDynamicMaterial()->SetScalarParameterValue(FName("Fill Percentage"), GetWorld()->GetTimerManager().GetTimerRemaining(Timers[4]) / it.Value);
			break;
		case 6:
			Cooldown6->GetDynamicMaterial()->SetScalarParameterValue(FName("Fill Percentage"), GetWorld()->GetTimerManager().GetTimerRemaining(Timers[6]) / it.Value);
			break;
		default:
			break;
		}
	}
}

