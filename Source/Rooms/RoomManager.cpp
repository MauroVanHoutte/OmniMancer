#include "Rooms/RoomManager.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include "Pickups/ElementPerkPickup.h"
#include "Pickups/HealthIncreasePickup.h"
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

bool ARoomManager::HasEncounterStarted()
{
	return bHasStarted;
}

void ARoomManager::StartEncounter()
{
	bHasStarted = true;

	switch (Type)
	{
	case RoomType::FireReward:
	case RoomType::IceReward:
	case RoomType::WindReward:
	case RoomType::HealReward:
	case RoomType::Shop:
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

void ARoomManager::StartBossEncounter_Implementation()
{
	OnEncounterCompleted();
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
		SpawnHealthReward();
		break;
	case RoomType::Boss:
		SpawnBossReward();
		break;
	case RoomType::Shop:
		OpenShop();
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
		UActorPoolingSubsystem* PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
		AElementPerkPickup* PerkPickup = Cast<AElementPerkPickup>(PoolingSystem->GetActorFromPool(PerkPickupClass));
		PerkPickup->SetActorLocationAndRotation(GetActorLocation(), FRotator(0, 0, 0));
		if (IsValid(PerkPickup))
		{
			PerkPickup->OnPickupClaimedDelegate.AddUniqueDynamic(this, &ARoomManager::OnPerkRewardClaimed);
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
	}
	else
	{
		ActivatePortals();
	}
}

void ARoomManager::SpawnBossReward()
{
	ActivatePortals();
}

void ARoomManager::SpawnHealthReward()
{
	UActorPoolingSubsystem* PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
	AHealthIncreasePickup* HealthPickup = Cast<AHealthIncreasePickup>(PoolingSystem->GetActorFromPool(HealthPickupClass));
	HealthPickup->SetActorLocationAndRotation(GetActorLocation(), FRotator(0, 0, 0));
	HealthPickup->OnPickupClaimedDelegate.AddUniqueDynamic(this, &ARoomManager::OnPerkRewardClaimed);
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

void ARoomManager::OpenShop_Implementation()
{
	ActivatePortals();
}

void ARoomManager::OnPerkRewardClaimed(AActor* ClaimingPlayer)
{
	ActivatePortals();
}