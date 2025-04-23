// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ScreenOverlay.h"
#include <Blueprint/UserWidget.h>
#include "UI/OverlayWidget.h"
// Sets default values for this component's properties
UScreenOverlay::UScreenOverlay()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UScreenOverlay::FadeInOverlay(float Duration)
{
	if (IsValid(Widget))
	{
		Widget->FadeInOverlay(Duration);
	}
}

void UScreenOverlay::FadeOutOverlay(float Duration)
{
	if (IsValid(Widget))
	{
		Widget->FadeOutOverlay(Duration);
	}
}


// Called when the game starts
void UScreenOverlay::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(WidgetClass))
	{
		APlayerController* Controller = GetOwner()->GetInstigatorController<APlayerController>();
		if (IsValid(Controller))
		{
			Widget = CreateWidget<UOverlayWidget>(Controller, WidgetClass);
			Widget->AddToPlayerScreen(1000);
			Widget->OnFadeCompletedDelegate.AddDynamic(this, &UScreenOverlay::OnWidgetFadeCompleted);
		}
	}
}

void UScreenOverlay::OnWidgetFadeCompleted()
{
	OnFadeComponentCompletedSignature.Broadcast();
}
