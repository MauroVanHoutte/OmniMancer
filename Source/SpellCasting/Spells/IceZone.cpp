// Fill out your copyright notice in the Description page of Project Settings.


#include "IceZone.h"
#include "Enemies/BaseCharacter.h"
#include "Health/AffiliationComponent.h"
#include "StatusEffects/StatusEffect.h"
#include "StatusEffects/StatusEffectHandlingComponent.h"
#include "WizardCharacter.h"

// Sets default values
AIceZone::AIceZone()
{
	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	RootComponent = CylinderMesh;
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder")).Object;
	CylinderMesh->SetStaticMesh(mesh);
}

void AIceZone::SetRadius(float radius)
{
	auto scale = CylinderMesh->GetRelativeScale3D();
	scale.X = radius;
	scale.Y = radius;
	CylinderMesh->SetRelativeScale3D(scale);
}

// Called when the game starts or when spawned
void AIceZone::BeginPlay()
{
	Damage = 0.f;

	CylinderMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ApplicationTimer, [this]()
		{
			SetActorEnableCollision(false);
			SetActorEnableCollision(true);
		}, ApplicationInterval, true);
}

void AIceZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(ApplicationTimer);
}

void AIceZone::OnDeath()
{
	GetWorld()->GetTimerManager().ClearTimer(ApplicationTimer);
}

bool AIceZone::WasActorHitBefore_Implementation(AActor* TriggeringActor)
{
	return false;
}

void AIceZone::InitSpell(const FVector& targetLocation, APawn* caster)
{
	Super::InitSpell(targetLocation, caster);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, targetLocation, targetLocation + FVector(0, 0, -500), ECollisionChannel::ECC_WorldStatic);
	SetActorLocation(Hit.bBlockingHit ? Hit.ImpactPoint : targetLocation);
	//SetBurnParams(ApplyBurn, BurnDamage + DamagePerFireLevel * FireLevel, BurnInterval, EffectLingerDuration + DurationPerFrostLevel * FrostLevel);
	//SetSlowParams(true, SlowAmount + SlowPerFrostLevel * FrostLevel, EffectLingerDuration);
	SetRadius(CircleScale);
}

