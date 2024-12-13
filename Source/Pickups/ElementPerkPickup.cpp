#include "ElementPerkPickup.h"
#include <Components/SphereComponent.h>
#include <Components/WidgetComponent.h>
#include <Components/Image.h>
#include "Pickups/InteractableComponent.h"
#include "UI/ElementTypeWidget.h"
#include "Upgrades/CharacterUpgradesComponent.h"

// Sets default values
AElementPerkPickup::AElementPerkPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	InteractactableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interaction"));
	InteractactableComponent->SetupAttachment(RootComponent);
	ElementWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ElementWidget"));
	ElementWidget->SetupAttachment(RootComponent);
}

void AElementPerkPickup::SetElement(WizardElement NewElement)
{
	Element = NewElement;
	if (IsValid(ElementWidget))
	{
		if (UElementTypeWidget* UserWidget = Cast<UElementTypeWidget>(ElementWidget->GetUserWidgetObject()))
		{
			UserWidget->SetElementType(Element);
		}
	}
}

void AElementPerkPickup::BeginPlay()
{
	Super::BeginPlay();
	InteractactableComponent->OnInteractActionDelegate.AddDynamic(this, &AElementPerkPickup::OnInteractAction);
	ElementWidget->SetWidgetClass(WidgetClass);
	SetElement(Element);
}

void AElementPerkPickup::OnInteractAction(AActor* Player)
{
	UCharacterUpgradesComponent* UpgradesComponent = Player->GetComponentByClass<UCharacterUpgradesComponent>();
	if (IsValid(UpgradesComponent))
	{
		UpgradesComponent->RequestPerkSelection(Element);
		OnPerkClaimedDelegate.Broadcast(Player);
		Destroy();
	}
}
