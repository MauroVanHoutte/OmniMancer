// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomPortal.h"
#include <Components/WidgetComponent.h>
#include <Components/Image.h>
#include <GameFramework/PlayerStart.h>
#include "Kismet/GameplayStatics.h"
#include "OmnimancerGameInstance.h"
#include "Pickups/InteractableComponent.h"
#include "Rooms/RoomManager.h"
#include "Rooms/RoomStreamingComponent.h"
#include "UI/RoomTypeWidget.h"
#include "UI/ScreenOverlay.h"
#include "WizardController.h"

// Sets default values
ARoomPortal::ARoomPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
	InteractableComponent->SetupAttachment(RootComponent);
	RoomStreamingComponent = CreateDefaultSubobject<URoomStreamingComponent>(TEXT("RoomStreaming"));
	PortalTypeWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PortalTypeWidget"));
	PortalTypeWidget->SetupAttachment(RootComponent);
}

void ARoomPortal::SetRoomType(RoomType Type)
{
	TargetRoomType = Type;
	if (IsValid(PortalTypeWidget))
	{
		if (URoomTypeWidget* UserWidget = Cast<URoomTypeWidget>(PortalTypeWidget->GetUserWidgetObject()))
		{
			UserWidget->SetRoomType(TargetRoomType);
		}
	}
}

void ARoomPortal::BeginPlay()
{
	Super::BeginPlay();
	InteractableComponent->OnComponentBeginOverlap.AddDynamic(this, &ARoomPortal::OnColliderBeginOverlap);
	InteractableComponent->OnComponentEndOverlap.AddDynamic(this, &ARoomPortal::OnColliderEndOverlap);
	RoomStreamingComponent->OnRoomLoadedDelegate.AddDynamic(this, &ARoomPortal::OnRoomLoaded);
	PortalTypeWidget->SetWidgetClass(PortalTypeWidgetClass);
	SetRoomType(TargetRoomType);
}

void ARoomPortal::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Contains(PlayerTag))
	{
		AWizardController* Controller = OtherActor->GetInstigatorController<AWizardController>();
		if (IsValid(Controller))
		{
			Controller->OnInteractActionDelegate.AddDynamic(this, &ARoomPortal::OnInteractAction);
		}
	}
}

void ARoomPortal::OnColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Tags.Contains(PlayerTag))
	{
		AWizardController* Controller = OtherActor->GetInstigatorController<AWizardController>();
		if (IsValid(Controller))
		{
			Controller->OnInteractActionDelegate.RemoveDynamic(this, &ARoomPortal::OnInteractAction);
		}
	}
}

void ARoomPortal::OnInteractAction(AActor* Player)
{
	InteractingPlayer = Player;
	UScreenOverlay* OverlayComponent = InteractingPlayer->GetComponentByClass<UScreenOverlay>();
	if (IsValid(OverlayComponent))
	{
		OverlayComponent->FadeInOverlay(1.f);
		OverlayComponent->OnFadeComponentCompletedSignature.AddDynamic(this, &ARoomPortal::OnFadeCompleted);
	}
	else
	{
		OnFadeCompleted();
	}

	AWizardController* Controller = Player->GetInstigatorController<AWizardController>();
	if (IsValid(Controller))
	{
		Controller->OnInteractActionDelegate.RemoveDynamic(this, &ARoomPortal::OnInteractAction);
	}
}

void ARoomPortal::OnRoomLoaded(ULevelStreaming* LoadedLevel)
{
	if (IsValid(LoadedLevel) && IsValid(LoadedLevel->GetLoadedLevel()))
	{
		TArray<ARoomPortal*> NewPortals;
		for (AActor* Actor : LoadedLevel->GetLoadedLevel()->Actors)
		{
			if (ARoomPortal* NewPortal = Cast<ARoomPortal>(Actor))
			{
				NewPortal->SetActorHiddenInGame(true);
				NewPortal->DisablePortal();
				NewPortals.Add(NewPortal);
			}
		}

		UOmnimancerGameInstance* GameInstance = GetGameInstance<UOmnimancerGameInstance>();
		if (IsValid(GameInstance))
		{
			GameInstance->SetPortalTypes(NewPortals);
			GameInstance->IncrementCompletedRoomCount();
		}

		for (AActor* Actor : LoadedLevel->GetLoadedLevel()->Actors)
		{
			if (ARoomManager* RoomManager = Cast<ARoomManager>(Actor))
			{
				RoomManager->SetRoomType(TargetRoomType);
			}
		}

		for (AActor* Actor : LoadedLevel->GetLoadedLevel()->Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				InteractingPlayer->SetActorLocation(PlayerStart->GetActorLocation(), false, nullptr, ETeleportType::ResetPhysics);
				break;
			}
		}
		
		UScreenOverlay* OverlayComponent = InteractingPlayer->GetComponentByClass<UScreenOverlay>();
		if (IsValid(OverlayComponent))
		{
			OverlayComponent->FadeOutOverlay(1);
		}
	}
}

void ARoomPortal::OnFadeCompleted()
{
	UScreenOverlay* OverlayComponent = InteractingPlayer->GetComponentByClass<UScreenOverlay>();
	if (IsValid(OverlayComponent))
	{
		OverlayComponent->OnFadeComponentCompletedSignature.RemoveDynamic(this, &ARoomPortal::OnFadeCompleted);
	}

	RoomStreamingComponent->UnloadPastRooms();
	RoomStreamingComponent->LoadNextRoom(TargetRoomType);
}
