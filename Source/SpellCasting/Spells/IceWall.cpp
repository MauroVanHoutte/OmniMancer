// Fill out your copyright notice in the Description page of Project Settings.


#include "IceWall.h"
#include "WizardCharacter.h"
#include "StatusEffects/StatusEffect.h"

AIceWall::AIceWall()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	Mesh->SetStaticMesh(cubeMesh);
	Mesh->SetCollisionProfileName(FName("BlockAll"));
	Mesh->SetCanEverAffectNavigation(true);
	
	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger->SetupAttachment(Mesh);
	Trigger->SetBoxExtent(FVector(55));
}

void AIceWall::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(WallThickness, WallWidth, WallHeight));
}

void AIceWall::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	
	if (RiseTimer < RiseTime)
	{
		RiseTimer += deltaTime;
		SetActorLocation(FMath::Lerp(StartLocation, TargetLocation, RiseTimer / RiseTime));
	}
}

void AIceWall::InitSpell(const FVector& targetLocation, APawn* caster)
{
	Super::InitSpell(targetLocation, caster);
	//SetSlowParams(true, SlowValue + SlowPerFrostLevel * FrostLevel, SlowDuration + DurationPerFrostLevel * FrostLevel);

	Damage = Damage + DamagePerFrostLevel * FrostLevel;

	//if (FrostLevel > 3)
	//	SetCurseParams(true, 15, 500, 5);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, targetLocation, targetLocation + FVector(0, 0, -500), ECollisionChannel::ECC_WorldStatic);
	FVector startLocation = Hit.bBlockingHit ? Hit.ImpactPoint : targetLocation;
	startLocation.Z -= (WallHeight*100)/2; //lower wall into floor

	TargetLocation = targetLocation;
	StartLocation = startLocation;
	SetActorLocation(startLocation);
	SetActorRotation(FRotator(0, (targetLocation - caster->GetActorLocation()).Rotation().Yaw, 0));
}
