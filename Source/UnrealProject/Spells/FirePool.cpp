// Fill out your copyright notice in the Description page of Project Settings.


#include "FirePool.h"
#include "../Enemies/BaseCharacter.h"

AFirePool::AFirePool()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	CylinderMesh->SetupAttachment(RootComponent);
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder")).Object;
	CylinderMesh->SetStaticMesh(mesh);
	CylinderMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AFirePool::InitSpell(const FVector& targetLocation, const FVector& projectileDirection, AWizardCharacter* wizard)
{
	ABaseSpell::InitSpell(targetLocation, projectileDirection, wizard);

	SetActorLocation(targetLocation);
	SetBurnParams(ApplyBurn, BurnDamage + BurnDamagePerFireLevel * FireLevel, BurnInterval, EffectLingerDuration);
	SetRadius(CircleScale + ScalePerFireLevel * FireLevel);
}

void AFirePool::SetRadius(float radius)
{
	auto scale = CylinderMesh->GetRelativeScale3D();
	scale.X = radius;
	scale.Y = radius;
	CylinderMesh->SetRelativeScale3D(scale);
}

void AFirePool::SetBurnCause(UObject* cause)
{
	for (FStatusEffect& effect : StatusEffects)
	{
		effect.Cause = cause;
	}
}

void AFirePool::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ApplicationTimer, [this]()
		{
			if (this == nullptr || !IsValidLowLevel() || IsPendingKill())
				return;

			TArray<AActor*> overlappingActors{};
			GetOverlappingActors(overlappingActors, ABaseCharacter::StaticClass());

			for (auto& actor : overlappingActors)
			{
				auto instigatorController = Cast<APlayerController>(GetInstigatorController());
				auto baseEnemy = Cast<ABaseCharacter>(actor);
				auto actorController = Cast<APlayerController>(baseEnemy->GetController());
				if (baseEnemy && instigatorController != actorController)
					baseEnemy->ReapplyStatusEffects(StatusEffects);
			}
		}, ApplicationInterval, true);
}

void AFirePool::OnDeath()
{
	GetWorld()->GetTimerManager().ClearTimer(ApplicationTimer);
}
