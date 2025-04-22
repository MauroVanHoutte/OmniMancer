// Fill out your copyright notice in the Description page of Project Settings.

#include "SpellCasting/ElementalAltar.h"
#include "Pickups/InteractableComponent.h"
#include "UI/ElementalAltarUi.h"
#include <Components/Button.h>

// Sets default values
AElementalAltar::AElementalAltar()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
	InteractableComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AElementalAltar::BeginPlay()
{
	Super::BeginPlay();

	AltarUi = CreateWidget<UElementalAltarUi>(GetWorld(), AltarUiClass);
	if (IsValid(AltarUi))
	{
		AltarUi->ExitButtonClickedDelegate.AddDynamic(this, &AElementalAltar::CloseUiMenu);
	}
	if (IsValid(InteractableComponent))
	{
		InteractableComponent->OnInteractActionDelegate.AddDynamic(this, &AElementalAltar::OnInteractAction);
		InteractableComponent->OnInteractRangeExitedDelegate.AddDynamic(this, &AElementalAltar::OnInteractRangeExited);
	}
}

void AElementalAltar::OnInteractAction(AActor* Player)
{
	if (IsValid(AltarUi))
	{
		AltarUi->SetPlayerActor(Player);
		AltarUi->SetPortal(Portal);
		AltarUi->AddToPlayerScreen(100000);
	}
}

void AElementalAltar::OnInteractRangeExited(AActor* Player)
{
	CloseUiMenu(nullptr);
}

void AElementalAltar::CloseUiMenu(UButton* ExitButton)
{
	AltarUi->RemoveFromParent();
}

