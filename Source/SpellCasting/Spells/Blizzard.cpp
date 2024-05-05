// Fill out your copyright notice in the Description page of Project Settings.


#include "Blizzard.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Enemies/BaseCharacter.h"
#include "WizardCharacter.h"
#include "StatusEffects/StatusEffect.h"
#include "StatusEffects/StatusEffectHandlingComponent.h"

ABlizzard::ABlizzard()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	CylinderMesh->SetupAttachment(RootComponent);
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder")).Object;
	CylinderMesh->SetStaticMesh(mesh);
}

void ABlizzard::TickBlizzard()
{
	ApplyWizardStats();

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, ABaseCharacter::StaticClass());
	for (AActor* Actor : OverlappingActors)
	{
		FDamageEvent DamageEvent;
		Actor->TakeDamage(DamagePerTick, DamageEvent, GetInstigatorController(), this);
		OnHit(Actor);
	}
}

void ABlizzard::SetWizard(AWizardCharacter* wizard)
{
	if (GetInstigator() != wizard)
	{
		SetInstigator(wizard);
		FAttachmentTransformRules attachRules{ EAttachmentRule::SnapToTarget, false };
		AttachToActor(wizard, attachRules);
		Wizard = wizard;
	}
}

void ABlizzard::ApplyWizardStats()
{
	InitSpell(FVector(), Wizard);
}

void ABlizzard::Activate()
{
	GetWorld()->GetTimerManager().SetTimer(ApplicationTimer, this, &ABlizzard::TickBlizzard, TickInterval, true, 0);
	CylinderMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CylinderMesh->SetVisibility(true);
}

void ABlizzard::Deactivate()
{
	GetWorld()->GetTimerManager().ClearTimer(ApplicationTimer);
	CylinderMesh->SetCollisionProfileName(TEXT("NoCollision"));
	CylinderMesh->SetVisibility(false);
}

void ABlizzard::BeginPlay()
{
	CylinderMesh->SetCollisionProfileName(TEXT("NoCollision"));
	CylinderMesh->SetVisibility(false);
}
