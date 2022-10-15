// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellIndicator.h"

// Sets default values
ASpellIndicator::ASpellIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	RootComponent = Root;

	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>("CylinderMesh");
	UStaticMesh* cylinderMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")).Object;
	CylinderMesh->SetStaticMesh(cylinderMesh);
	CylinderMesh->SetCollisionProfileName(FName("NoCollision"));
	CylinderMesh->SetCanEverAffectNavigation(false);
	CylinderMesh->bHiddenInGame = true;

	CylinderProgressMesh = CreateDefaultSubobject<UStaticMeshComponent>("CylinderProgressMesh");
	CylinderProgressMesh->SetStaticMesh(cylinderMesh);
	CylinderProgressMesh->SetCollisionProfileName(FName("NoCollision"));
	CylinderProgressMesh->SetCanEverAffectNavigation(false);
	CylinderProgressMesh->bHiddenInGame = true;
	

	CylinderMesh->SetupAttachment(Root);
	CylinderProgressMesh->SetupAttachment(Root);
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

	if (Active)
	{
		Timer += DeltaTime;
		FVector newScale = FMath::VInterpTo(FVector(0.1, 0.1, 0.1), Scale, Timer, 1/Duration); //inner cylinder will grow to the size of the constant cylinder to display when spells will hit
		CylinderProgressMesh->SetRelativeScale3D(newScale);
		if (Timer > Duration)
		{
			Destroy();
		}
	}

}

void ASpellIndicator::StartCircleIndicator(const FVector& scale, float lifetime)
{
	Active = true;
	CylinderMesh->SetRelativeScale3D(scale);
	CylinderProgressMesh->SetRelativeScale3D(FVector(0.1, 0.1, scale.Z));
	Duration = lifetime;
	Timer = 0.f;
	Scale = scale;

	CylinderMesh->bHiddenInGame = false;
	CylinderProgressMesh->bHiddenInGame = false;
}

