// Fill out your copyright notice in the Description page of Project Settings.


#include "FlameColumn.h"
#include "../Enemies/BaseCharacter.h"

// Sets default values
AFlameColumn::AFlameColumn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	RootComponent = m_CylinderMesh;
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder")).Object;
	m_CylinderMesh->SetStaticMesh(mesh);
	
}

// Called when the game starts or when spawned
void AFlameColumn::BeginPlay()
{
	m_LifeSpan = m_ImpactDelay + m_VisualLinger;

	Super::BeginPlay();
	m_CylinderMesh->SetRelativeScale3D(FVector(m_CircleScale, m_CircleScale, 1.f));
	m_CylinderMesh->SetCollisionProfileName(TEXT("NoCollision"));



	m_Damage = m_InitialDamage;
	if (m_ApplyBurn)
	{
		m_StatusEffects.Add(FStatusEffect(Type::Damage, m_BurnInterval, m_BurnDamage, m_BurnDuration, this));
	}
}

// Called every frame
void AFlameColumn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_ImpactDelay -= DeltaTime;
	if (m_ImpactDelay < 0)
	{
		m_CylinderMesh->SetRelativeScale3D(FVector(m_CircleScale, m_CircleScale, 100.f));
		m_CylinderMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	}
}


