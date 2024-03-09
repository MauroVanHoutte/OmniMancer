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

// Called when the game starts or when spawned
void AFlameColumn::BeginPlay()
{
	Super::BeginPlay();
	CylinderMesh->SetCollisionProfileName(TEXT("NoCollision"));

}

void AFlameColumn::OnDeath()
{
	if (FireLevel > 3) // flame columns with fire level 4 or higher spawn a new one on death with adjusted levels
	{
		auto spell = GetWorld()->SpawnActor<AFlameColumn>(GetClass()); 
		spell->InitSpell(GetActorLocation(), GetOwner<AWizardCharacter>()); 
		spell->FireLevel /= 2; // less damage
		spell->WindLevel *= 3; // bigger area
	}
}

// Called every frame
void AFlameColumn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ImpactDelay -= DeltaTime;
	if (ImpactDelay < 0 && !SetOff)
	{
		CylinderMesh->SetRelativeScale3D(FVector(CircleScale + ScalePerWindLevel * WindLevel, CircleScale + ScalePerWindLevel * WindLevel, 100.f));
		CylinderMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		SetOff = true;
	}
}

void AFlameColumn::InitSpell(const FVector& TargetLocation, APawn* Caster)
{
	Super::InitSpell(TargetLocation, Caster);

	Damage = InitialDamage + DamagePerFireLevel * FireLevel;
	SetActorLocation(TargetLocation);
	CylinderMesh->SetRelativeScale3D(FVector(CircleScale + ScalePerWindLevel * WindLevel, CircleScale + ScalePerWindLevel * WindLevel, 1.f));
	SetLifeTime(ImpactDelay + VisualLinger);
}

void AFlameColumn::OnHit(AActor* HitActor)
{
	Super::OnHit(HitActor);
}


