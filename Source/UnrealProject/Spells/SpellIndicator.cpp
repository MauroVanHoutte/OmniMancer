// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellIndicator.h"

// Sets default values
ASpellIndicator::ASpellIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	RootComponent = m_Root;

	m_CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>("CylinderMesh");
	UStaticMesh* cylinderMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")).Object;
	m_CylinderMesh->SetStaticMesh(cylinderMesh);
	m_CylinderMesh->SetCollisionProfileName(FName("NoCollision"));
	m_CylinderMesh->SetCanEverAffectNavigation(false);
	m_CylinderMesh->bHiddenInGame = true;

	m_CylinderProgressMesh = CreateDefaultSubobject<UStaticMeshComponent>("CylinderProgressMesh");
	m_CylinderProgressMesh->SetStaticMesh(cylinderMesh);
	m_CylinderProgressMesh->SetCollisionProfileName(FName("NoCollision"));
	m_CylinderProgressMesh->SetCanEverAffectNavigation(false);
	m_CylinderProgressMesh->bHiddenInGame = true;
	
	m_BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>("BoxMesh");
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	m_BoxMesh->SetStaticMesh(cubeMesh);
	m_BoxMesh->SetCollisionProfileName(FName("NoCollision"));
	m_BoxMesh->SetCanEverAffectNavigation(false);
	m_BoxMesh->bHiddenInGame = true;

	m_BoxProgressMesh = CreateDefaultSubobject<UStaticMeshComponent>("BoxProgressMesh");
	m_BoxProgressMesh->SetStaticMesh(cubeMesh);
	m_BoxProgressMesh->SetCollisionProfileName(FName("NoCollision"));
	m_BoxProgressMesh->SetCanEverAffectNavigation(false);
	m_BoxProgressMesh->bHiddenInGame = true;

	m_CylinderMesh->SetupAttachment(m_Root);
	m_CylinderProgressMesh->SetupAttachment(m_Root);
	m_BoxMesh->SetupAttachment(m_Root);
	m_BoxProgressMesh->SetupAttachment(m_Root);
}

// Called when the game starts or when spawned
void ASpellIndicator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpellIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_Active)
	{
		m_Timer += DeltaTime;
		FVector newScale = FMath::VInterpTo(FVector(0.1, 0.1, 0.1), m_Scale, m_Timer, 1/m_Duration); //inner cylinder will grow to the size of the constant cylinder to display when spells will hit
		m_CylinderProgressMesh->SetRelativeScale3D(newScale);
		if (m_Timer > m_Duration)
		{
			Destroy();
		}
	}

}

void ASpellIndicator::StartCircleIndicator(const FVector& scale, float lifetime)
{
	m_Active = true;
	m_CylinderMesh->SetRelativeScale3D(scale);
	m_CylinderProgressMesh->SetRelativeScale3D(FVector(0.1, 0.1, scale.Z));
	m_Duration = lifetime;
	m_Timer = 0.f;
	m_Scale = scale;

	m_CylinderMesh->bHiddenInGame = false;
	m_CylinderProgressMesh->bHiddenInGame = false;
}

