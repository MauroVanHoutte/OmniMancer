// Fill out your copyright notice in the Description page of Project Settings.


#include "Tornado.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "../Enemies/BaseCharacter.h"
#include "../WizardCharacter.h"
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

void ATornado::InitSpell(const FVector& targetLocation, const FVector& projectileDirection, AWizardCharacter* wizard)
{
	Super::InitSpell(targetLocation, projectileDirection, wizard);

	SetActorLocation(wizard->GetActorLocation());
	FireInDirection(projectileDirection);

	Damage = BaseDamage + DamagePerFireLevel * FireLevel;
	ScaleGrowth += ScaleGrowthPerWindLevel * WindLevel;
	SetStunParams(true, StunDuration + StunDurationPerFrostLevel * FireLevel);
}

void ATornado::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	Mesh->SetWorldScale3D(FVector(StartScale, StartScale, StartScale));
}

void ATornado::OnHit(ABaseCharacter* hitActor)
{
	Super::OnHit(hitActor);

	hitActor->Knockup();

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
	lightning->InitSpell(targetActor->GetActorLocation(), (targetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal(), Cast<AWizardCharacter>(GetInstigator()));
	lightning->SetDamageMultiplier(0.5f); //weaker damage
}

void ATornado::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		Destroy();
	}
}
