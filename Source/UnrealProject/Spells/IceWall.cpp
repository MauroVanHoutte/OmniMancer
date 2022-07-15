// Fill out your copyright notice in the Description page of Project Settings.


#include "IceWall.h"

AIceWall::AIceWall()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	Mesh->SetStaticMesh(cubeMesh);
	Mesh->SetCollisionProfileName(FName("BlockAll"));
	Mesh->SetCanEverAffectNavigation(true);
	
	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger->SetupAttachment(Mesh);
	Trigger->SetBoxExtent(FVector(55));
}

void AIceWall::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(WallThickness, WallWidth, WallHeight));
}

void AIceWall::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	
	if (RiseTimer < RiseTime)
	{
		RiseTimer += deltaTime;
		SetActorLocation(FMath::Lerp(StartLocation, TargetLocation, RiseTimer / RiseTime));
	}
}

void AIceWall::InitSpell(const FVector& casterLocation, const FVector& targetLocation, const FVector& projectileDirection, AActor* owner, APawn* instigator, int fireLevel, int frostLevel, int windLevel)
{
	Super::InitSpell(casterLocation, targetLocation, projectileDirection, owner, instigator, fireLevel, frostLevel, windLevel);
	SetSlowParams(true, 50, 3);

	if (FrostLevel > 3)
		SetCurseParams(true, 15, 500, 5);

	FVector startLocation = targetLocation;
	startLocation.Z -= (WallHeight*100)/2; //lower wall into floor

	TargetLocation = targetLocation;
	StartLocation = startLocation;
	SetActorLocation(startLocation);
	SetActorRotation(FRotator(0, (targetLocation - casterLocation).Rotation().Yaw, 0));
}
