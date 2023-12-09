// Fill out your copyright notice in the Description page of Project Settings.


#include "Blizzard.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Enemies/BaseCharacter.h"
#include "../WizardCharacter.h"

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

	TArray<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors, ABaseCharacter::StaticClass());
	for (AActor* actor : overlappingActors)
	{
		ABaseCharacter* character = Cast<ABaseCharacter>(actor);
		if (!character->IsA<AWizardCharacter>())
		{
			character->TakeSpellDamage(this);
			character->ReapplyStatusEffects(GetStatusEffects());
			OnHit(character);
		}
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
	SetSlowParams(true, SlowAmount, TickInterval);
	Damage = DamagePerTick;
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
