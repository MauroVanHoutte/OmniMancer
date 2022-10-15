// Fill out your copyright notice in the Description page of Project Settings.


#include "IceZone.h"
#include "../Enemies/BaseCharacter.h"
#include "../WizardCharacter.h"

// Sets default values
AIceZone::AIceZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void AIceZone::Destroy()
{
	AActor::Destroy();
}

// Called when the game starts or when spawned
void AIceZone::BeginPlay()
{
	Damage = 0.f;
	CylinderMesh->SetRelativeScale3D(FVector(CircleScale, CircleScale, 1.f));

	CylinderMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	Super::BeginPlay();

}

// Called every frame
void AIceZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ApplicationTimer += DeltaTime;
	if (ApplicationTimer > ApplicationInterval)
	{
		ApplicationTimer -= ApplicationInterval;

		TArray<AActor*> overlappingActors{};
		GetOverlappingActors(overlappingActors);

		for (auto& actor : overlappingActors)
		{
			auto instigatorController = Cast<APlayerController>(GetInstigatorController());
			auto baseEnemy = Cast<ABaseCharacter>(actor);
			if (!baseEnemy)
				continue;
			auto actorController = Cast<APlayerController>(baseEnemy->GetController());
			if (baseEnemy &&  instigatorController != actorController)
				baseEnemy->ReapplyStatusEffects(StatusEffects);
		}
	}

}

void AIceZone::InitSpell(const FVector& targetLocation, const FVector& projectileDirection, AWizardCharacter* wizard)
{
	Super::InitSpell(targetLocation, projectileDirection, wizard);

	SetActorLocation(targetLocation);
	SetBurnParams(ApplyBurn, BurnDamage + DamagePerFireLevel * FireLevel, BurnInterval, EffectLingerDuration + DurationPerFrostLevel * FrostLevel);
	SetSlowParams(true, SlowAmount + SlowPerFrostLevel * FrostLevel, EffectLingerDuration);
	SetRadius(7.f);
}

