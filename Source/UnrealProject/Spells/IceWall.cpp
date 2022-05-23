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
	
}

void AIceWall::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetRelativeScale3D(FVector(1.f, WallWidth, 3.f));
}

void AIceWall::InitSpell(const FVector& casterLocation, const FVector& targetLocation, const FVector& projectileDirection, AActor* owner, APawn* instigator, int fireLevel, int frostLevel, int windLevel)
{
	Super::InitSpell(casterLocation, targetLocation, projectileDirection, owner, instigator, fireLevel, frostLevel, windLevel);

	SetActorLocation(targetLocation);
	SetActorRotation((targetLocation - casterLocation).Rotation());
}
