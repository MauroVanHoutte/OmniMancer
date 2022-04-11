// Fill out your copyright notice in the Description page of Project Settings.


#include "IceWall.h"

AIceWall::AIceWall()
{
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = m_Mesh;
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	m_Mesh->SetStaticMesh(cubeMesh);
	m_Mesh->SetCollisionProfileName(FName("BlockAll"));
	m_Mesh->SetCanEverAffectNavigation(true);
	
}

void AIceWall::BeginPlay()
{
	Super::BeginPlay();
	m_Mesh->SetRelativeScale3D(FVector(1.f, m_WallWidth, 3.f));
}
