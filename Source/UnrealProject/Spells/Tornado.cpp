// Fill out your copyright notice in the Description page of Project Settings.


#include "Tornado.h"
#include "../Enemies/BaseCharacter.h"

ATornado::ATornado()
{
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto cylinderMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")).Object;
	m_Mesh->SetStaticMesh(cylinderMesh);
	m_Mesh->SetCollisionProfileName("OverlapAll");
	m_Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	m_Mesh->OnComponentBeginOverlap.AddDynamic(this, &ATornado::OnHit);
	RootComponent = m_Mesh;

	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	m_ProjectileMovement->SetUpdatedComponent(RootComponent);
	m_ProjectileMovement->bShouldBounce = false;
	m_ProjectileMovement->ProjectileGravityScale = 0.f;

}

void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto scale = m_Mesh->GetRelativeScale3D();
	float newScale = scale.X + m_ScaleGrowth * DeltaTime;
	m_Mesh->SetWorldScale3D(FVector(newScale, newScale, m_StartScale));
}

void ATornado::FireInDirection(const FVector& direction)
{
	m_ProjectileMovement->Velocity = direction * m_ProjectileMovement->InitialSpeed;
}

void ATornado::BeginPlay()
{
	m_Damage = m_TornadoDamage;
	Super::BeginPlay();
	m_ProjectileMovement->InitialSpeed = m_Speed;
	m_ProjectileMovement->MaxSpeed = m_Speed;
	m_Mesh->SetWorldScale3D(FVector(m_StartScale, m_StartScale, m_StartScale));

	m_StatusEffects.Add(FStatusEffect(Type::Stun, -1.f, -1.f, m_StunDuration, this));
}

void ATornado::OnHit(AActor* hitActor)
{
	auto enemy = Cast<ABaseCharacter>(hitActor);
	if (enemy != nullptr)
		enemy->Knockup();
}

void ATornado::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		Destroy();
	}
}
