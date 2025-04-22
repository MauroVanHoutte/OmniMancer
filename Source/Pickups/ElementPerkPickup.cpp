#include "ElementPerkPickup.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include <Components/SphereComponent.h>
#include <Components/WidgetComponent.h>
#include <Components/Image.h>
#include "Pickups/InteractableComponent.h"
#include "UI/ElementTypeWidget.h"
#include "Upgrades/CharacterUpgradesComponent.h"

void AElementPerkPickup::SetElement(WizardElement NewElement)
{
	Element = NewElement;
	if (IsValid(Widget))
	{
		if (UElementTypeWidget* UserWidget = Cast<UElementTypeWidget>(Widget->GetUserWidgetObject()))
		{
			UserWidget->SetElementType(Element);
		}
	}
}

void AElementPerkPickup::BeginPlay()
{
	Super::BeginPlay();
	SetElement(Element);
}

void AElementPerkPickup::OnInteractAction_Implementation(AActor* Player)
{
	UCharacterUpgradesComponent* UpgradesComponent = Player->GetComponentByClass<UCharacterUpgradesComponent>();
	if (IsValid(UpgradesComponent))
	{
		UpgradesComponent->RequestPerkSelection(Element);
		OnPickupClaimedDelegate.Broadcast(Player);

		ReturnToPoolOrDestroy();
	}
}
