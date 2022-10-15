// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../Enemies/BaseCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "../WizardCharacter.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "NiagaraFunctionLibrary.h"

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
	ProjectileMovement->InitialSpeed = MaxSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
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
	
	//interrupt ai movement to allow knowback
	SetStunParams(true, 0.1f);
}

void ABaseProjectile::Explode()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionParticle, GetActorLocation());
	TArray<AActor*> outActors{};
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ExplosionRadius * 100, TArray<TEnumAsByte<EObjectTypeQuery>>{}, ABaseCharacter::StaticClass(), TArray<AActor*>{GetInstigator()}, outActors);
	for (AActor* actor : outActors)
	{
		Cast<ABaseCharacter>(actor)->TakeSpellDamageFloat(ExplosionDamage, GetInstigator());
	}
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::InitSpell(const FVector& targetLocation, const FVector& projectileDirection, AWizardCharacter* wizard)
{
	Super::InitSpell(targetLocation, projectileDirection, wizard);

	SetDamageMultiplier(wizard->GetBaseProjecileDamageMultiplier() * wizard->GetSpellDamageMultiplier());
	TotalBounces = wizard->GetBounces();
	SetActorLocationAndRotation(wizard->GetActorLocation(), projectileDirection.Rotation());
	FireInDirection(projectileDirection);
}

void ABaseProjectile::FireInDirection(const FVector& direction)
{
	ProjectileMovement->Velocity = direction * ProjectileMovement->InitialSpeed;
	SetActorRotation(direction.Rotation());
}

void ABaseProjectile::OnHit(AActor* hitActor)
{
	Super::OnHit(hitActor);

	Cast<ABaseCharacter>(hitActor)->Push(ProjectileMovement->Velocity.GetSafeNormal() * PushbackForce);

	if (Explosive)
	{
		Explode();
	}

	if (HitParticle != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticle, hitActor->GetActorLocation(), GetActorRotation());
	}

	if (BounceCount < TotalBounces)
	{
		TArray<AActor*> actors;
		TArray<AActor*> ignore{ Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn()) };
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), BounceRange, TArray<TEnumAsByte<EObjectTypeQuery>>(), ABaseCharacter::StaticClass(), ignore, actors);

		if (actors.Num() > 0)
		{
			//sort near to far
			actors.Sort([this]( const AActor& A, const AActor& B) { return FVector::DistSquared(A.GetActorLocation(), GetActorLocation()) < FVector::DistSquared(B.GetActorLocation(), GetActorLocation()); });
			for (AActor* actor : actors)
			{
				FHitResult hit{};
				if (!WasActorHit(actor) && !GetWorld()->LineTraceSingleByObjectType(hit, GetActorLocation(), actor->GetActorLocation(), ECollisionChannel::ECC_WorldStatic))
				{
					BounceCount++;
					FireInDirection((actor->GetActorLocation() - GetActorLocation()).GetSafeNormal());
					return;
				}
			}
		}
	}
	Destroy();
}

void ABaseProjectile::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Initialized && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
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



