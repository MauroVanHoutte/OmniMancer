// Fill out your copyright notice in the Description page of Project Settings.


#include "FlameColumn.h"
#include "../Enemies/BaseCharacter.h"

// Sets default values
AFlameColumn::AFlameColumn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	RootComponent = CylinderMesh;
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder")).Object;
	CylinderMesh->SetStaticMesh(mesh);
	
}

// Called when the game starts or when spawned
void AFlameColumn::BeginPlay()
{
	LifeSpan = ImpactDelay + VisualLinger;

	Super::BeginPlay();
	CylinderMesh->SetRelativeScale3D(FVector(CircleScale, CircleScale, 1.f));
	CylinderMesh->SetCollisionProfileName(TEXT("NoCollision"));



	Damage = InitialDamage;
	if (ApplyBurn)
	{
		StatusEffects.Add(FStatusEffect(Type::Damage, BurnInterval, BurnDamage, BurnDuration, this));
	}
}

// Called every frame
void AFlameColumn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ImpactDelay -= DeltaTime;
	if (ImpactDelay < 0)
	{
		CylinderMesh->SetRelativeScale3D(FVector(CircleScale, CircleScale, 100.f));
		CylinderMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	}
}


