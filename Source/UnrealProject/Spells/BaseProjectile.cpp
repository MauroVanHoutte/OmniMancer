// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	m_CollisionComponent->InitSphereRadius(30.f);
	m_CollisionComponent->SetGenerateOverlapEvents(true);
	m_CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	m_CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
	m_CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnHit);
	RootComponent = m_CollisionComponent;

	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	m_ProjectileMovement->SetUpdatedComponent(m_CollisionComponent);
	m_ProjectileMovement->InitialSpeed = 1000.f;
	m_ProjectileMovement->MaxSpeed = 1000.f;
	m_ProjectileMovement->bShouldBounce = false;
	m_ProjectileMovement->ProjectileGravityScale = 0.f;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	m_Mesh->SetStaticMesh(sphereMesh);
	m_Mesh->SetWorldScale3D(FVector(1.5f, 0.5f, 0.5f));
	m_Mesh->SetupAttachment(RootComponent);
	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	InitialLifeSpan = m_LifeTime;
	m_Damage = 5;
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::FireInDirection(const FVector& direction)
{
	m_ProjectileMovement->Velocity = direction * m_ProjectileMovement->InitialSpeed;
}

void ABaseProjectile::OnHit(AActor* hitActor)
{
	Destroy();
}

void ABaseProjectile::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		Destroy();
	}
}



