// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"
#include "Coin.h"
#include "PlayerTriggerInterace.h"
#include "Upgrades/ExperienceComponent.h"

UPickupComponent::UPickupComponent()
{
	OnComponentBeginOverlap.AddDynamic(this, &UPickupComponent::OnBeginOverlap);
}

void UPickupComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPlayerTriggerInterface* PlayerTriggerInterface = Cast<IPlayerTriggerInterface>(OtherActor);

	if (PlayerTriggerInterface)
	{
		PlayerTriggerInterface->Execute_OnTriggeredByPlayer(OtherActor, GetOwner());
	}
}