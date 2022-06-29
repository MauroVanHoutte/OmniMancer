// Fill out your copyright notice in the Description page of Project Settings.


#include "FlameColumn.h"
#include "../Enemies/BaseCharacter.h"

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

// Called when the game starts or when spawned
void AFlameColumn::BeginPlay()
{
	Super::BeginPlay();
	CylinderMesh->SetCollisionProfileName(TEXT("NoCollision"));

}

void AFlameColumn::OnDeath()
{
	if (FireLevel > 3)
	{
		auto spell = GetWorld()->SpawnActor<AFlameColumn>(GetClass());
		spell->InitSpell(FVector(), GetActorLocation(), FVector(), GetOwner(), GetInstigator(), 3, FrostLevel, WindLevel * 2); //bigger radius less damage
	}
}

// Called every frame
void AFlameColumn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ImpactDelay -= DeltaTime;
	if (ImpactDelay < 0)
	{
		CylinderMesh->SetRelativeScale3D(FVector(CircleScale + ScalePerWindLevel * WindLevel, CircleScale + ScalePerWindLevel * WindLevel, 100.f));
		CylinderMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	}
}

void AFlameColumn::InitSpell(const FVector& casterLocation, const FVector& targetLocation, const FVector& projectileDirection, AActor* owner, APawn* instigator, int fireLevel, int frostLevel, int windLevel)
{
	Super::InitSpell(casterLocation, targetLocation, projectileDirection, owner, instigator, fireLevel, frostLevel, windLevel);

	Damage = InitialDamage + DamagePerFireLevel * fireLevel;
	SetActorLocation(targetLocation);
	SetBurnParams(ApplyBurn, BurnDamage + BurnDamagePerFireLevel * fireLevel, BurnInterval, BurnDuration + DurationPerFrostLevel * frostLevel);
	CylinderMesh->SetRelativeScale3D(FVector(CircleScale + ScalePerWindLevel*windLevel, CircleScale + ScalePerWindLevel*windLevel, 1.f));
	SetLifeTime(ImpactDelay + VisualLinger);
}


