// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainLightning.h"
#include <Kismet/GameplayStatics.h>
#include "../Enemies/BaseCharacter.h"
#include "../WizardCharacter.h"

AChainLightning::AChainLightning()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	m_CollisionComponent->InitSphereRadius(30.f);
	m_CollisionComponent->SetGenerateOverlapEvents(true);
	m_CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AChainLightning::OnHit);
	RootComponent = m_CollisionComponent;

	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	m_ProjectileMovement->SetUpdatedComponent(m_CollisionComponent);
	m_ProjectileMovement->InitialSpeed = 5000.f;
	m_ProjectileMovement->MaxSpeed = 5000.f;
	m_ProjectileMovement->bShouldBounce = false;
	m_ProjectileMovement->ProjectileGravityScale = 0.f;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	m_Mesh->SetStaticMesh(sphereMesh);
	m_Mesh->SetWorldScale3D(FVector(1.5f, 0.5f, 0.5f));
	m_Mesh->SetupAttachment(RootComponent);
	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

void AChainLightning::BeginPlay()
{
	InitialLifeSpan = m_LifeTime;
	Super::BeginPlay();
	m_Damage = m_InitialDamage;
	if (m_Stuns)
	{
		m_StatusEffects.Add(FStatusEffect(Type::Stun, -1.f, -1.f, m_StunDuration, this));
	}
}

void AChainLightning::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		Destroy();
	}
}

void AChainLightning::Tick(float DeltaTime)
{
}

void AChainLightning::FireInDirection(const FVector& direction)
{
	SetActorRotation(direction.Rotation());
	m_ProjectileMovement->Velocity = direction * m_ProjectileMovement->InitialSpeed;
}

void AChainLightning::OnHit(AActor* hitActor)
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TSubclassOf<ABaseCharacter>(ABaseCharacter::StaticClass()), actors);

	for ( AActor* actor : actors)
	{
		if (Cast<AWizardCharacter>(actor) != nullptr)
			continue;

		if (!WasActorHit(actor) && FVector::DistSquared(actor->GetActorLocation(), GetActorLocation()) < m_BounceRange*m_BounceRange)
		{
			FireInDirection((actor->GetActorLocation() - GetActorLocation()).GetSafeNormal());
			return;
		}
	}

	Destroy();
}
