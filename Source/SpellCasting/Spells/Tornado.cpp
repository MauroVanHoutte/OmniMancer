// Fill out your copyright notice in the Description page of Project Settings.


#include "Tornado.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "Enemies/BaseCharacter.h"
#include "WizardCharacter.h"
#include "ChainLightning.h"
#include "Movement/ForceApplicationComponent.h"

ATornado::ATornado()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto cylinderMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")).Object;
	Mesh->SetStaticMesh(cylinderMesh);
	Mesh->SetCollisionProfileName("NoCollision");
	RootComponent = Mesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->SetUpdatedComponent(RootComponent);
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

}

void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto scale = Mesh->GetRelativeScale3D();
	float newScale = scale.X + ScaleGrowth * DeltaTime;
	Mesh->SetWorldScale3D(FVector(newScale, newScale, StartScale));
}

void ATornado::FireInDirection(const FVector& direction)
{
	ProjectileMovement->Velocity = direction * ProjectileMovement->InitialSpeed;
}

void ATornado::InitSpell(const FVector& targetLocation, APawn* caster)
{
	Super::InitSpell(targetLocation, caster);
	Mesh->SetWorldScale3D(FVector(StartScale, StartScale, StartScale));
	FireInDirection((targetLocation - caster->GetActorLocation()).GetSafeNormal());
}

void ATornado::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	Mesh->SetWorldScale3D(FVector(StartScale, StartScale, StartScale));
	Mesh->UpdateOverlaps();
}

void ATornado::OnHit(AActor* HitActor)
{
	Super::OnHit(HitActor);

	UForceApplicationComponent* ForceComponent = HitActor->GetComponentByClass<UForceApplicationComponent>();
	if (IsValid(ForceComponent))
		ForceComponent->ApplyImpulse(FVector(0, 0, 50000));
}
