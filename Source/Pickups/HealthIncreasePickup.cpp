// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/HealthIncreasePickup.h"
#include "Health/BaseHealthComponent.h"
#include "Health/HealthManager.h"

void AHealthIncreasePickup::OnInteractAction_Implementation(AActor* Player)
{
	if (IsValid(Player))
	{
		UHealthManager* HealthManager = Player->GetComponentByClass<UHealthManager>();
		if (IsValid(HealthManager))
		{
			TArray<UBaseHealthComponent*>& HealthComponents = HealthManager->GetHealthComponents();
			UBaseHealthComponent* HealthComponent = HealthComponents.Last();
			HealthComponent->SetMaxHealth(HealthComponent->GetMaxHealth() + BonusHealth);
			ReturnToPoolOrDestroy();
		}
	}
}
