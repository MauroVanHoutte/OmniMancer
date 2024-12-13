// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/InteractableComponent.h"
#include <Components/WidgetComponent.h>
#include <Engine/LocalPlayer.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include "UI/InteractWidget.h"
#include "WizardController.h"

UInteractableComponent::UInteractableComponent()
{
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
}

void UInteractableComponent::OnRegister()
{
	Super::OnRegister();
	Widget->SetupAttachment(this);
}

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnEndOverlap);
	Widget->SetWidgetClass(WidgetClass);
	Widget->SetVisibility(false);
}

void UInteractableComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Contains(PlayerTag))
	{
		AWizardController* Controller = OtherActor->GetInstigatorController<AWizardController>();
		if (IsValid(Controller))
		{
			Controller->OnInteractActionDelegate.AddDynamic(this, &UInteractableComponent::OnInteractAction);
			SetMappedKeyOnWidget(Controller);
			Widget->SetVisibility(true);
			OnInteractRangeEnteredDelegate.Broadcast(OtherActor);
		}
	}
}

void UInteractableComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Tags.Contains(PlayerTag))
	{
		AWizardController* Controller = OtherActor->GetInstigatorController<AWizardController>();
		if (IsValid(Controller))
		{
			Controller->OnInteractActionDelegate.RemoveDynamic(this, &UInteractableComponent::OnInteractAction);
			Widget->SetVisibility(false);
			OnInteractRangeExitedDelegate.Broadcast(OtherActor);
		}
	}
}

void UInteractableComponent::OnInteractAction(AActor* Player)
{
	OnInteractActionDelegate.Broadcast(Player);
}

void UInteractableComponent::SetMappedKeyOnWidget(AWizardController* Controller)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer()))
	{
		TArray<FKey> Keys = Subsystem->QueryKeysMappedToAction(InteractAction);
		UInteractWidget* UserWidget = Cast<UInteractWidget>(Widget->GetUserWidgetObject());
		if (IsValid(UserWidget) && !Keys.IsEmpty())
		{
			UserWidget->SetKeyText(FText::FromName(Keys[0].GetFName()));
		}
	}
}
