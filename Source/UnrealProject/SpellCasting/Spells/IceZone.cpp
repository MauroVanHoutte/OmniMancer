// Fill out your copyright notice in the Description page of Project Settings.


#include "IceZone.h"
#include "Enemies/BaseCharacter.h"
#include "WizardCharacter.h"
#include "StatusEffects/StatusEffect.h"
#include "StatusEffects/StatusEffectHandlingComponent.h"

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
			TArray<AActor*> overlappingActors{};
			GetOverlappingActors(overlappingActors, ABaseCharacter::StaticClass());

			for (AActor* actor : overlappingActors)
			{
				/*auto instigatorController = Cast<APlayerController>(GetInstigatorController());
				auto baseEnemy = Cast<ABaseCharacter>(actor);
				auto actorController = Cast<APlayerController>(baseEnemy->GetController());*/
				/*if (baseEnemy && instigatorController != actorController)
					baseEnemy->ReapplyStatusEffects(StatusEffects);*/
				OnHit(actor);
			}
		}, ApplicationInterval, true);
}

void AIceZone::OnDeath()
{
	GetWorld()->GetTimerManager().ClearTimer(ApplicationTimer);
}

void AIceZone::InitSpell(const FVector& targetLocation, APawn* caster)
{
	Super::InitSpell(targetLocation, caster);

	SetActorLocation(targetLocation);
	//SetBurnParams(ApplyBurn, BurnDamage + DamagePerFireLevel * FireLevel, BurnInterval, EffectLingerDuration + DurationPerFrostLevel * FrostLevel);
	//SetSlowParams(true, SlowAmount + SlowPerFrostLevel * FrostLevel, EffectLingerDuration);
	SetRadius(CircleScale);
}

