// Fill out your copyright notice in the Description page of Project Settings.


#include "FlameColumn.h"
#include "Enemies/BaseCharacter.h"
#include "WizardCharacter.h"
#include "StatusEffects/StatusEffectPoolingSubsystem.h"
#include "StatusEffects/StatusEffectHandlingComponent.h"

// Sets default values
AFlameColumn::AFlameColumn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	RootComponent = CylinderMesh;
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder")).Object;
	CylinderMesh->SetStaticMesh(mesh);
}

void AFlameColumn::OnActorTakenFromPool_Implementation()
{
	Super::OnActorTakenFromPool_Implementation();
	CircleScale = DefaultCircleScale;
}

// Called when the game starts or when spawned
void AFlameColumn::BeginPlay()
{
	Super::BeginPlay();
	CylinderMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called every frame
void AFlameColumn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ImpactTimer -= DeltaTime;
	if (ImpactTimer < 0 && !SetOff)
	{
		CylinderMesh->SetRelativeScale3D(FVector(CircleScale, CircleScale, 100.f));
		CylinderMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		SetOff = true;
	}
}

void AFlameColumn::InitSpell(const FVector& TargetLocation, APawn* Caster)
{
	CylinderMesh->SetCollisionProfileName(TEXT("NoCollision"));
	Super::InitSpell(TargetLocation, Caster);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, TargetLocation, TargetLocation + FVector(0, 0, -500), ECollisionChannel::ECC_WorldStatic);
	SetActorLocation(Hit.bBlockingHit ? Hit.ImpactPoint : TargetLocation);	

	CylinderMesh->SetRelativeScale3D(FVector(CircleScale, CircleScale, 1.f));
	SetOff = false;
	ImpactTimer = ImpactDelay;
	SetLifeSpan(ImpactDelay + VisualLinger);
}

void AFlameColumn::SetScale(float NewScale)
{
	CircleScale = NewScale;
	FVector Scale = CylinderMesh->GetRelativeScale3D();
	CylinderMesh->SetRelativeScale3D(FVector(CircleScale, CircleScale, Scale.Z));
}

void AFlameColumn::OnHit(AActor* HitActor)
{
	Super::OnHit(HitActor);
}


