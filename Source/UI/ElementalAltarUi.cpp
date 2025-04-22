// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ElementalAltarUi.h"
#include <Components/Button.h>
#include "Rooms/RoomPortal.h"
#include "SpellCasting/ElementManipulationComponent.h"

void UElementalAltarUi::NativeConstruct()
{
	FireButton->OnClicked.AddUniqueDynamic(this, &UElementalAltarUi::FireButtonClicked);
	IceButton->OnClicked.AddUniqueDynamic(this, &UElementalAltarUi::IceButtonClicked);
	WindButton->OnClicked.AddUniqueDynamic(this, &UElementalAltarUi::WindButtonClicked);
	ExitButton->OnClicked.AddUniqueDynamic(this, &UElementalAltarUi::ExitButtonClicked);
}

void UElementalAltarUi::SetPlayerActor(AActor* PlayerActor)
{
	InteractingPlayerSpellcasting = PlayerActor->GetComponentByClass<UElementManipulationComponent>();
	if (IsValid(InteractingPlayerSpellcasting))
	{
		TSet<WizardElement> LearnedElements = InteractingPlayerSpellcasting->GetLearnedElements();
		if (!LearnedElements.IsEmpty())
		{
			TArray<WizardElement> AllElements{WizardElement::Fire, WizardElement::Frost, WizardElement::Wind};
			for (WizardElement LearnedElement : LearnedElements)
			{
				AllElements.Remove(LearnedElement);
				switch (LearnedElement)
				{
				case WizardElement::None:
					break;
				case WizardElement::Fire:
					FireButton->SetIsEnabled(false);
					break;
				case WizardElement::Frost:
					IceButton->SetIsEnabled(false);
					break;
				case WizardElement::Wind:
					WindButton->SetIsEnabled(false);
					break;
				default:
					break;
				}
			}
			for (WizardElement NonLearnedElement : AllElements)
			{
				switch (NonLearnedElement)
				{
				case WizardElement::None:
					break;
				case WizardElement::Fire:
					FireButton->SetIsEnabled(true);
					break;
				case WizardElement::Frost:
					IceButton->SetIsEnabled(true);
					break;
				case WizardElement::Wind:
					WindButton->SetIsEnabled(true);
					break;
				default:
					break;
				}
			}
		}
	}
}

void UElementalAltarUi::SetPortal(ARoomPortal* RoomPortal)
{
	Portal = RoomPortal;
}

void UElementalAltarUi::FireButtonClicked()
{
	if (IsValid(InteractingPlayerSpellcasting))
	{
		InteractingPlayerSpellcasting->UnlearnElement(WizardElement::Frost);
		InteractingPlayerSpellcasting->UnlearnElement(WizardElement::Wind);
		InteractingPlayerSpellcasting->LearnElement(WizardElement::Fire);
		InteractingPlayerSpellcasting->AddElement(WizardElement::Fire);
		InteractingPlayerSpellcasting->AddElement(WizardElement::Fire);
		FireButton->SetIsEnabled(false);
		IceButton->SetIsEnabled(true);
		WindButton->SetIsEnabled(true);
		if (IsValid(Portal))
		{
			Portal->SetRoomType(RoomType::FireReward);
		}
	}
}

void UElementalAltarUi::IceButtonClicked()
{
	if (IsValid(InteractingPlayerSpellcasting))
	{
		InteractingPlayerSpellcasting->UnlearnElement(WizardElement::Fire);
		InteractingPlayerSpellcasting->UnlearnElement(WizardElement::Wind);
		InteractingPlayerSpellcasting->LearnElement(WizardElement::Frost);
		InteractingPlayerSpellcasting->AddElement(WizardElement::Frost);
		InteractingPlayerSpellcasting->AddElement(WizardElement::Frost);
		IceButton->SetIsEnabled(false);
		FireButton->SetIsEnabled(true);
		WindButton->SetIsEnabled(true);
		if (IsValid(Portal))
		{
			Portal->SetRoomType(RoomType::IceReward);
		}
	}
}

void UElementalAltarUi::WindButtonClicked()
{
	if (IsValid(InteractingPlayerSpellcasting))
	{
		InteractingPlayerSpellcasting->UnlearnElement(WizardElement::Fire);
		InteractingPlayerSpellcasting->UnlearnElement(WizardElement::Frost);
		InteractingPlayerSpellcasting->LearnElement(WizardElement::Wind);
		InteractingPlayerSpellcasting->AddElement(WizardElement::Wind);
		InteractingPlayerSpellcasting->AddElement(WizardElement::Wind);
		WindButton->SetIsEnabled(false);
		FireButton->SetIsEnabled(true);
		IceButton->SetIsEnabled(true);
		if (IsValid(Portal))
		{
			Portal->SetRoomType(RoomType::WindReward);
		}
	}
}

void UElementalAltarUi::ExitButtonClicked()
{
	ExitButtonClickedDelegate.Broadcast(ExitButton);
}
