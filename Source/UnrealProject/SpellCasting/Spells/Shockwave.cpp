// Fill out your copyright notice in the Description page of Project Settings.


#include "Shockwave.h"
#include "Enemies/BaseCharacter.h"
#include "WizardCharacter.h"

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
	
	scale.X = scale.Y += FMath::Min(deltaTime * ScaleGrowth, MaxScale);

	if (scale.X >= MaxScale)
	{
		scale.X = scale.Y = MaxScale;
		SetActorTickEnabled(false);
	}

	StaticMeshComponent->SetRelativeScale3D(scale);
}

void AShockwave::InitSpell(const FVector& targetLocation, APawn* caster)
{
	Super::InitSpell(targetLocation, caster);

	SetActorLocation(caster->GetActorLocation());
	//SetStunParams(true, StunDuration + StunDurationPerFrostLevel * FrostLevel);
	Damage = InitialDamage + DamagePerWindLevel * WindLevel;
	KnockbackAmount += KnockbackPerWindLevel * WindLevel;
}

void AShockwave::BeginPlay()
{
	Super::BeginPlay();
}

void AShockwave::OnHit(AActor* hitActor)
{
	Super::OnHit(hitActor);

	auto spellOrigin = GetActorLocation();
	spellOrigin.Z -= 200.f; // push will be slightly upwards

	auto direcrion = (hitActor->GetActorLocation() - spellOrigin).GetSafeNormal();

	//hitActor->Push(direcrion*KnockbackAmount);
}


