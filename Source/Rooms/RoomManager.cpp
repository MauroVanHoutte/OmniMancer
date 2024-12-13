#include "Rooms/RoomManager.h"
#include "Pickups/ElementPerkPickup.h"
#include <Kismet/GameplayStatics.h>
#include "Rooms/RoomPortal.h"

// Sets default values
ARoomManager::ARoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ARoomManager::SetRoomType(RoomType NewType)
{
	Type = NewType;
}

void ARoomManager::StartEncounter()
{
	switch (Type)
	{
	case RoomType::FireReward:
	case RoomType::IceReward:
	case RoomType::WindReward:
	case RoomType::HealReward:
		StartEnemyWavesEncounter();
		break;
	case RoomType::Boss:
		StartBossEncounter();
		break;
	default:
		SpawnReward();
		break;
	}
}

void ARoomManager::OnEncounterCompleted()
{
	SpawnReward();
}

void ARoomManager::StartEnemyWavesEncounter_Implementation()
{
	OnEncounterCompleted();
}

void ARoomManager::StartBossEncounter()
{
}

void ARoomManager::SpawnReward()
{
	switch (Type)
	{
	case RoomType::FireReward:
	case RoomType::IceReward:
	case RoomType::WindReward:
		SpawnElementReward();
		break;
	case RoomType::HealReward:
		break;
	case RoomType::Boss:
		break;
	default:
		ActivatePortals();
		break;
	}
}

void ARoomManager::SpawnElementReward()
{
	if (IsValid(PerkPickupClass))
	{
		AElementPerkPickup* PerkPickup = GetWorld()->SpawnActor<AElementPerkPickup>(PerkPickupClass, GetActorLocation(), FRotator(0,0,0));
		PerkPickup->OnPerkClaimedDelegate.AddDynamic(this, &ARoomManager::OnPerkRewardClaimed);
		switch (Type)
		{
		case RoomType::FireReward:
			PerkPickup->SetElement(WizardElement::Fire);
			break;
		case RoomType::IceReward:
			PerkPickup->SetElement(WizardElement::Frost);
			break;
		case RoomType::WindReward:
			PerkPickup->SetElement(WizardElement::Wind);
			break;
		default:
			break;
		}
	}
	else
	{
		ActivatePortals();
	}
}

void ARoomManager::ActivatePortals()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoomPortal::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		if (ARoomPortal* Portal = Cast<ARoomPortal>(Actor))
		{
			Portal->EnablePortal();
		}
	}
}

void ARoomManager::OnPerkRewardClaimed(AActor* ClaimingPlayer)
{
	ActivatePortals();
}