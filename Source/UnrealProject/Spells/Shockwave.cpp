// Fill out your copyright notice in the Description page of Project Settings.


#include "Shockwave.h"
#include "../Enemies/BaseCharacter.h"

AShockwave::AShockwave()
{
	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//auto torusMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Torus.Torus'")).Object;

	//m_StaticMeshComponent->SetStaticMesh(torusMesh);
	m_StaticMeshComponent->SetCollisionProfileName("OverlapAll");
	m_StaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	RootComponent = m_StaticMeshComponent;

}

void AShockwave::Tick(float deltaTime)
{
	auto scale = m_StaticMeshComponent->GetRelativeScale3D();
	scale.X += deltaTime * m_ScaleGrowth;
	scale.Y += deltaTime * m_ScaleGrowth;

	m_StaticMeshComponent->SetRelativeScale3D(scale);
}

void AShockwave::BeginPlay()
{
	m_Damage = m_InitialDamage;
	Super::BeginPlay();

}

void AShockwave::OnHit(AActor* hitActor)
{
	auto enemy = Cast<ABaseCharacter>(hitActor);

	auto spellOrigin = GetActorLocation();
	spellOrigin.Z -= 200.f; // push will be slightly upwards

	auto direcrion = (enemy->GetActorLocation() - spellOrigin).GetSafeNormal();

	if (enemy != nullptr)
		enemy->Push(direcrion*m_KnockbackAmount);
}


