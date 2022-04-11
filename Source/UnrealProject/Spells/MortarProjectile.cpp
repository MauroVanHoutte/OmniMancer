// Fill out your copyright notice in the Description page of Project Settings.


#include "MortarProjectile.h"
#include "../WizardCharacter.h"

AMortarProjectile::AMortarProjectile()
{
	m_CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	m_CollisionComponent->InitSphereRadius(m_ProjectileRadius);
	m_CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMortarProjectile::OnHit);
	m_CollisionComponent->SetCollisionProfileName("OverlapAll");
	RootComponent = m_CollisionComponent;

	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	m_ProjectileMovement->SetUpdatedComponent(m_CollisionComponent);
	m_ProjectileMovement->bShouldBounce = false;
	m_ProjectileMovement->ProjectileGravityScale = 1.f;
	m_ProjectileMovement->InitialSpeed = m_ProjectileSpeedZ;
	m_ProjectileMovement->MaxSpeed = m_ProjectileSpeedZ * 2;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	m_Mesh->SetStaticMesh(mesh);
	m_Mesh->SetupAttachment(RootComponent);
	m_Mesh->SetCollisionProfileName("NoCollision");
}

UProjectileMovementComponent* AMortarProjectile::GetProjectileMovementComp()
{
	return m_ProjectileMovement;
}

void AMortarProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AMortarProjectile::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		m_CollisionComponent->SetCollisionProfileName("NoCollision");
		SpawnExplosion(GetActorLocation());
		Destroy();
	}
}
