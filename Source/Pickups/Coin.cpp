// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include "WizardCharacter.h"
#include "OmnimancerGameInstance.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/SphereComponent.h>
#include "Upgrades/ExperienceComponent.h"
#include "Upgrades/StatUpgrades/StatComponent.h"

// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComponent->InitSphereRadius(10.f);
	RootComponent = CollisionComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	Mesh->SetStaticMesh(mesh);
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(HomeInTarget))
	{
		return;
	}

	Time += DeltaTime;

	//coins are attracted towards player
	const FVector Delta = HomeInTarget->GetActorLocation() - GetActorLocation();
	const float DeltaM = Delta.Size();
	const float MaxStep = Speed->GetFloatValue(Time) * DeltaTime;
	const FVector DeltaN = Delta / DeltaM;
	const FVector NewLocation = GetActorLocation() + DeltaN * MaxStep;

	SetActorLocation(NewLocation);
}

void ACoin::LaunchInDirection(const FVector& direction, float strength)
{
	CollisionComponent->AddImpulse(direction * strength, NAME_None, true);
}

void ACoin::HomeInToTarget(AActor* Target)
{
	if (!IsValid(HomeInTarget))
	{
		HomeInTarget = Target;
	}
	CollisionComponent->SetEnableGravity(false);
}

int ACoin::GetValue()
{
	return Value;
}

void ACoin::OnTriggeredByPlayer_Implementation(AActor* Player)
{
	if (bIsExperience)
	{
		UExperienceComponent* ExperienceComponent = Player->GetComponentByClass<UExperienceComponent>();

		if (IsValid(ExperienceComponent))
			ExperienceComponent->AddExperience(Value);
	}
	else
	{
		UStatComponent* Stats = Player->GetComponentByClass<UStatComponent>();
		if (IsValid(Stats))
			Stats->SetCurrency(Stats->GetCurrency() + Value);
	}



	HomeInTarget = nullptr;
	Time = 0;
	CollisionComponent->SetEnableGravity(true);
	ReturnToPoolOrDestroy();
}

