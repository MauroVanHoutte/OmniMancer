// Fill out your copyright notice in the Description page of Project Settings.


#include "Shockwave.h"
#include "../Enemies/BaseCharacter.h"

AShockwave::AShockwave()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//auto torusMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Torus.Torus'")).Object;

	//m_StaticMeshComponent->SetStaticMesh(torusMesh);
	StaticMeshComponent->SetCollisionProfileName("OverlapAll");
	StaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	RootComponent = StaticMeshComponent;

}

void AShockwave::Tick(float deltaTime)
{
	auto scale = StaticMeshComponent->GetRelativeScale3D();
	scale.X += deltaTime * ScaleGrowth;
	scale.Y += deltaTime * ScaleGrowth;

	StaticMeshComponent->SetRelativeScale3D(scale);
}

void AShockwave::InitSpell(const FVector& casterLocation, const FVector& targetLocation, const FVector& projectileDirection, AActor* owner, APawn* instigator, int fireLevel, int frostLevel, int windLevel)
{
	SetOwner(owner);
	SetInstigator(instigator);
	SetActorLocation(casterLocation);
	SetStunParams(true, StunDuration + StunDurationPerFrostLevel * frostLevel);
	Damage = InitialDamage + DamagePerWindLevel * windLevel;
	KnockbackAmount += KnockbackPerWindLevel * windLevel;
}

void AShockwave::BeginPlay()
{
	Super::BeginPlay();
}

void AShockwave::OnHit(AActor* hitActor)
{
	auto enemy = Cast<ABaseCharacter>(hitActor);

	auto spellOrigin = GetActorLocation();
	spellOrigin.Z -= 200.f; // push will be slightly upwards

	auto direcrion = (enemy->GetActorLocation() - spellOrigin).GetSafeNormal();

	if (enemy != nullptr)
		enemy->Push(direcrion*KnockbackAmount);
}


