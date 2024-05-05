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

	FireInDirection((targetLocation - caster->GetActorLocation()).GetSafeNormal());

	Damage = BaseDamage + DamagePerFireLevel * FireLevel;
	ScaleGrowth += ScaleGrowthPerWindLevel * WindLevel;
	//SetStunParams(true, StunDuration + StunDurationPerFrostLevel * FireLevel);
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

	//hitActor->Knockup();
	UForceApplicationComponent* ForceComponent = HitActor->GetComponentByClass<UForceApplicationComponent>();
	ForceComponent->ApplyImpulse(FVector(0, 0, 50000));

	//Wind level 4 effect
	if (WindLevel >= 4)
		ShootLightning(HitActor);
}

void ATornado::ShootLightning(AActor* targetActor)
{
	if (Lightning == nullptr)
		return;
	auto lightning = GetWorld()->SpawnActor<AChainLightning>(Lightning);
	lightning->InitSpell(targetActor->GetActorLocation(), Cast<AWizardCharacter>(GetInstigator()));
	lightning->FireInDirection((targetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal());
	lightning->SetDamageMultiplier(0.5f); //weaker damage
}
