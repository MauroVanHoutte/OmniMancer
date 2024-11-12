// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellCasting/Spells/Explosion.h"
#include <Components/SphereComponent.h>
#include "Movement/ForceApplicationComponent.h"

AExplosion::AExplosion()
	:ABaseSpell()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Mesh->SetupAttachment(RootComponent);
}

void AExplosion::InitSpell(const FVector& targetLocation, APawn* caster)
{
	FullSphereSize = CollisionSphere->GetScaledSphereRadius();
	FullMeshScale = Mesh->GetRelativeScale3D().X;
	CollisionSphere->SetSphereRadius(0.01f);
	Mesh->SetRelativeScale3D(FVector(0.01f, 0.01f, 0.01f));
	SphereGrowSpeed = FullSphereSize / GrowDuration;
	MeshGrowSpeed = FullMeshScale / GrowDuration;
	Timer = 0;
	SetLifeSpan(GrowDuration + LingerDuration);

	Super::InitSpell(targetLocation, caster);
}

void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if (Timer >= GrowDuration)
	{
		Timer = GrowDuration;
	}

	CollisionSphere->SetSphereRadius(FullSphereSize * (Timer / GrowDuration));
	float MeshScale = FullMeshScale * (Timer / GrowDuration);
	Mesh->SetRelativeScale3D(FVector(MeshScale, MeshScale, MeshScale));
}

void AExplosion::OnHit(AActor* hitActor)
{
	Super::OnHit(hitActor);

	UForceApplicationComponent* Comp = hitActor->GetComponentByClass<UForceApplicationComponent>();
	if (IsValid(Comp))
	{
		FVector Direction = (hitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		Direction.Z += AddedUpwardsDirection;
		Direction.Normalize();
		Comp->ApplyImpulse(Direction * PushStrength);
	}
}
