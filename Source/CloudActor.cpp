// Fill out your copyright notice in the Description page of Project Settings.


#include "CloudActor.h"
#include "WizardCharacter.h"

// Sets default values
ACloudActor::ACloudActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>("CollisionCapsule");
	RootComponent = CollisionComponent;

	CloudMesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	CloudMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACloudActor::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACloudActor::OnBeginOverlap);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACloudActor::CheckOverlapBegin);
}

void ACloudActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Contains("Player"))
	{
		IsRising = true;
	}
}

void ACloudActor::CheckOverlapBegin()
{
	TArray<AActor*> overlappingActors{};
	CollisionComponent->GetOverlappingActors(overlappingActors);
	for (AActor* actor : overlappingActors)
	{
		if (actor->IsA(AWizardCharacter::StaticClass()))
			Destroy();
	}
}

// Called every frame
void ACloudActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRising)
	{
		FVector location = CloudMesh->GetRelativeLocation();
		location.Z += RisePerSecond * DeltaTime;
		CloudMesh->SetRelativeLocation(location);
		FVector scale = CloudMesh->GetRelativeScale3D();
		scale.X -= ShrinkPerSecond * DeltaTime;
		scale.Y -= ShrinkPerSecond * DeltaTime;
		scale.Z -= ShrinkPerSecond * DeltaTime;
		CloudMesh->SetRelativeScale3D(scale);
		if (scale.X < 0)
			Destroy();
	}
}

