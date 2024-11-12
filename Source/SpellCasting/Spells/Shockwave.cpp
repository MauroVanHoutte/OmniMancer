// Fill out your copyright notice in the Description page of Project Settings.


#include "Shockwave.h"

#include <Components/SphereComponent.h>
#include "Enemies/BaseCharacter.h"
#include "Movement/ForceApplicationComponent.h"
#include "WizardCharacter.h"

AShockwave::AShockwave()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(50.f);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
	RootComponent = CollisionComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	StaticMeshComponent->SetCollisionProfileName("NoCollision");
	StaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void AShockwave::Tick(float deltaTime)
{
	GrowthTimer += deltaTime;
	float CurrentScale = FMath::Lerp(1, MaxScaleMultiplier, GrowthTimer/GrowthDuration);

	if (GrowthTimer > GrowthDuration)
	{
		SetActorTickEnabled(false);
	}

	CollisionComponent->SetSphereRadius(InitialRadius * CurrentScale);
	
	StaticMeshComponent->SetRelativeScale3D(FVector(CurrentScale, CurrentScale, 1));
}

void AShockwave::InitSpell(const FVector& targetLocation, APawn* caster)
{
	Super::InitSpell(targetLocation, caster);

	SetActorLocation(caster->GetActorLocation());
	SetLifeSpan(GrowthDuration + LingerDuration);
}

void AShockwave::BeginPlay()
{
	Super::BeginPlay();
}

void AShockwave::OnHit(AActor* HitActor)
{
	Super::OnHit(HitActor);

	auto SpellOrigin = GetActorLocation();
	SpellOrigin.Z -= 200.f; // push will be slightly upwards

	auto Direction = (HitActor->GetActorLocation() - SpellOrigin).GetSafeNormal();

	UForceApplicationComponent* ForceComponent = HitActor->GetComponentByClass<UForceApplicationComponent>();
	if (IsValid(ForceComponent))
		ForceComponent->ApplyImpulse(Direction * KnockbackAmount);
}


