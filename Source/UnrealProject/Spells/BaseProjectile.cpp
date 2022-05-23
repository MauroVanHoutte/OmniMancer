// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../Enemies/BaseCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "../WizardCharacter.h"
#include <Components/SphereComponent.h>
#include "../ParticleActor.h"
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(30.f);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnHit);
	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->SetUpdatedComponent(CollisionComponent);
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	Mesh->SetStaticMesh(sphereMesh);
	Mesh->SetWorldScale3D(FVector(1.5f, 0.5f, 0.5f));
	Mesh->SetupAttachment(CollisionComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	Damage = 5;
}

void ABaseProjectile::Explode()
{
	auto particleActor = GetWorld()->SpawnActor<AParticleActor>(ParticleActorClass);
	particleActor->SetSystem(ExplosionSystem, 0.5f);
	particleActor->SetActorScale3D(FVector(ExplosionRadius));
	particleActor->SetActorLocation(GetActorLocation());
	TArray<AActor*> outActors{};
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ExplosionRadius * 100, TArray<TEnumAsByte<EObjectTypeQuery>>{}, ABaseCharacter::StaticClass(), TArray<AActor*>{GetOwner()}, outActors);
	for (AActor* actor : outActors)
	{
		Cast<ABaseCharacter>(actor)->TakeSpellDamage(ExplosionDamage);
	}
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::FireInDirection(const FVector& direction)
{
	ProjectileMovement->Velocity = direction * ProjectileMovement->InitialSpeed;
	SetActorRotation(direction.Rotation());
}

void ABaseProjectile::OnHit(AActor* hitActor)
{
	if (Explosive)
	{
		Explode();
	}

	if (BounceCount < TotalBounces)
	{
		TArray<AActor*> actors;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), BounceRange, TArray<TEnumAsByte<EObjectTypeQuery>>(), ABaseCharacter::StaticClass(), TArray<AActor*>(), actors);

		for (AActor* actor : actors)
		{
			if (Cast<AWizardCharacter>(actor) != nullptr)
				continue;

			if (!WasActorHit(actor))
			{
				BounceCount++;
				FireInDirection((actor->GetActorLocation() - GetActorLocation()).GetSafeNormal());
				return;
			}
		}
	}
	Destroy();
}

void ABaseProjectile::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		if(Explosive)
			Explode();
		Destroy();
	}
}

void ABaseProjectile::SetExplosion(float radius, float damage)
{
	Explosive = true;
	ExplosionDamage = damage;
	ExplosionRadius = radius;
}

void ABaseProjectile::SetBounces(int bounces)
{
	TotalBounces = bounces;
}



