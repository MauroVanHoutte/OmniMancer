// Fill out your copyright notice in the Description page of Project Settings.


#include "MortarProjectile.h"
#include "WizardCharacter.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/SphereComponent.h>

AMortarProjectile::AMortarProjectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComponent->InitSphereRadius(ProjectileRadius);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMortarProjectile::OnHit);
	CollisionComponent->SetCollisionProfileName("OverlapAll");
	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->SetUpdatedComponent(CollisionComponent);
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 1.f;
	ProjectileMovement->InitialSpeed = ProjectileSpeedZ;
	ProjectileMovement->MaxSpeed = ProjectileSpeedZ * 2;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	Mesh->SetStaticMesh(mesh);
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName("NoCollision");
}

UProjectileMovementComponent* AMortarProjectile::GetProjectileMovementComp()
{
	return ProjectileMovement;
}

void AMortarProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AMortarProjectile::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		CollisionComponent->SetCollisionProfileName("NoCollision"); //will collide multiple times before actually being destroyed
		if (IsValid(this))
			SpawnExplosion(GetActorLocation());
		Destroy();
	}
}
