// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "WizardCharacter.h"
#include "OmnimancerGameInstance.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/SphereComponent.h>

// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComponent->InitSphereRadius(10.f);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnOverlap);
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

void ACoin::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AWizardCharacter>() && HomeInTarget->IsValidLowLevel())
	{
		if (IsExperience)
		{
			//Player->PickupExperience(Value);
		}
		else //currency
		{
			auto gameInstance = Cast<UOmnimancerGameInstance>(GetGameInstance());
			gameInstance->SetCurrency(gameInstance->GetCurrency() + Value);
		}
		Destroy();
	}
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
}

