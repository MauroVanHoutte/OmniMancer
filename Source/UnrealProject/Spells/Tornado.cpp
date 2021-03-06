// Fill out your copyright notice in the Description page of Project Settings.


#include "Tornado.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "../Enemies/BaseCharacter.h"
#include "ChainLightning.h"

ATornado::ATornado()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto cylinderMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")).Object;
	Mesh->SetStaticMesh(cylinderMesh);
	Mesh->SetCollisionProfileName("OverlapAll");
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ATornado::OnHit);
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

void ATornado::InitSpell(const FVector& casterLocation, const FVector& targetLocation, const FVector& projectileDirection, AActor* owner, APawn* instigator, int fireLevel, int frostLevel, int windLevel)
{
	Super::InitSpell(casterLocation, targetLocation, projectileDirection, owner, instigator, fireLevel, frostLevel, windLevel);

	SetActorLocation(casterLocation);
	FireInDirection(projectileDirection);

	Damage += DamagePerFireLevel * fireLevel;
	ScaleGrowth += ScaleGrowthPerWindLevel * windLevel;
	SetStunParams(true, StunDuration + (StunDurationPerFrostLevel * frostLevel));
}

void ATornado::BeginPlay()
{
	Damage = BaseDamage;
	Super::BeginPlay();
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	Mesh->SetWorldScale3D(FVector(StartScale, StartScale, StartScale));
}

void ATornado::OnHit(AActor* hitActor)
{
	auto enemy = Cast<ABaseCharacter>(hitActor);
	if (enemy != nullptr)
		enemy->Knockup();

	//Wind level 4 effect
	if (WindLevel >= 4)
		ShootLightning(hitActor);
}

void ATornado::ShootLightning(AActor* targetActor)
{
	if (Lightning == nullptr)
		return;
	auto lightning = GetWorld()->SpawnActor<AChainLightning>(Lightning);
	FVector caster = GetActorLocation();
	caster.Z += 100.f;
	lightning->InitSpell(caster, targetActor->GetActorLocation(), (targetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal(), GetOwner(), GetInstigator(), 1, 1, WindLevel / 2); //lowered levels
}

void ATornado::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		Destroy();
	}
}
