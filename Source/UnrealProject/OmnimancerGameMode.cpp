// Fill out your copyright notice in the Description page of Project Settings.


#include "OmnimancerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "OmnimancerGameInstance.h"


void AOmnimancerGameMode::BeginPlay()
{
	TArray<AActor*> taggedActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MainSpawnPoint"), taggedActors);
	if (taggedActors.Num() > 0)
	{
		auto gameInstance = GetGameInstance<UOmnimancerGameInstance>();
		WizardElement wizardType = gameInstance->GetSelectedType();

		AWizardCharacter* player = nullptr;

		switch (wizardType)
		{
		case WizardElement::Fire:
			if (FireWizardBlueprint != nullptr)
			{
				player = GetWorld()->SpawnActor<AWizardCharacter>(FireWizardBlueprint.Get(), taggedActors[0]->GetTransform());
			}
			break;
		case WizardElement::Frost:
			if (FrostWizardBlueprint != nullptr)
			{
				player = GetWorld()->SpawnActor<AWizardCharacter>(FrostWizardBlueprint.Get(), taggedActors[0]->GetTransform());
			}
			break;
		case WizardElement::Wind:
			if (WindWizardBlueprint != nullptr)
			{
				player = GetWorld()->SpawnActor<AWizardCharacter>(WindWizardBlueprint.Get(), taggedActors[0]->GetTransform());
			}
			break;
		default:
			break;
		}

		if (player != nullptr)
		{
			GetWorld()->GetFirstPlayerController()->Possess(player);
		}
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RoomSpawnPointBlueprint, RoomSpawnPoints);
}

void AOmnimancerGameMode::NextRoom()
{
	if (RoomSpawnPoints.Num() > 0)
	{
		int32 idx = FMath::RandRange(0, RoomSpawnPoints.Num() - 1);
		auto player = GetWorld()->GetFirstPlayerController()->GetPawn();
		player->SetActorLocation(RoomSpawnPoints[idx]->GetActorLocation());
	}
}
