// Fill out your copyright notice in the Description page of Project Settings.


#include "FirePool.h"
#include "Enemies/BaseCharacter.h"
#include "StatusEffects/StatusEffect.h"

AFirePool::AFirePool()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	CylinderMesh->SetupAttachment(RootComponent);
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder")).Object;
	CylinderMesh->SetStaticMesh(mesh);
	CylinderMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Damage = 1.f;
}

void AFirePool::InitSpell(const FVector& targetLocation, APawn* caster)
{
	ABaseSpell::InitSpell(targetLocation, caster);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, targetLocation, targetLocation + FVector(0, 0, -500), ECollisionChannel::ECC_WorldStatic);
	SetActorLocation(Hit.bBlockingHit ? Hit.ImpactPoint : targetLocation);

	SetRadius(CircleScale);
}

void AFirePool::SetRadius(float radius)
{
	auto scale = CylinderMesh->GetRelativeScale3D();
	scale.X = radius;
	scale.Y = radius;
	CylinderMesh->SetRelativeScale3D(scale);
}

void AFirePool::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ApplicationTimer, [this]()
		{
			if (this == nullptr || !IsValidLowLevel())
				return;

			TArray<AActor*> OverlappingActors{};
			GetOverlappingActors(OverlappingActors, ABaseCharacter::StaticClass());

			for (AActor* Actor : OverlappingActors)
			{
				FDamageEvent DamageEvent;
				Actor->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
				OnHit(Actor);
			}
		}, ApplicationInterval, true);
}

void AFirePool::OnDeath()
{
	GetWorld()->GetTimerManager().ClearTimer(ApplicationTimer);
}
