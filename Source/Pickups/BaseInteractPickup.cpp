// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/BaseInteractPickup.h"
#include <Components/SphereComponent.h>
#include <Components/WidgetComponent.h>
#include "Pickups/InteractableComponent.h"

ABaseInteractPickup::ABaseInteractPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	InteractactableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interaction"));
	InteractactableComponent->SetupAttachment(RootComponent);
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);
}

void ABaseInteractPickup::OnActorTakenFromPool_Implementation()
{
	Super::OnActorTakenFromPool_Implementation();
	Widget->SetVisibility(true);
}

void ABaseInteractPickup::OnActorReturnedToPool_Implementation()
{
	Super::OnActorReturnedToPool_Implementation();
	Widget->SetVisibility(false);
	Widget->UpdateWidget();
}

void ABaseInteractPickup::BeginPlay()
{
	Super::BeginPlay();
	InteractactableComponent->OnInteractActionDelegate.AddDynamic(this, &ABaseInteractPickup::OnInteractAction);
	Widget->SetWidgetClass(WidgetClass);
}

void ABaseInteractPickup::OnInteractAction_Implementation(AActor* Player)
{

}