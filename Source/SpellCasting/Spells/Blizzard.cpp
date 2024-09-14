// Fill out your copyright notice in the Description page of Project Settings.


#include "Blizzard.h"
#include "Enemies/BaseCharacter.h"
#include "Health/AffiliationComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "StatusEffects/StatusEffect.h"
#include "StatusEffects/StatusEffectHandlingComponent.h"
#include "WizardCharacter.h"

ABlizzard::ABlizzard()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	CylinderMesh->SetupAttachment(RootComponent);
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder")).Object;
	CylinderMesh->SetStaticMesh(mesh);
}

bool ABlizzard::WasActorHitBefore_Implementation(AActor* TriggeringActor)
{
	return false;
}

void ABlizzard::TickBlizzard()
{
	ApplyWizardStats();
	SetActorEnableCollision(false);
	SetActorEnableCollision(true);
}

void ABlizzard::SetParent(APawn* Parent)
{
	SetInstigator(Parent);
	FAttachmentTransformRules attachRules{ EAttachmentRule::SnapToTarget, false };
	AttachToActor(Parent, attachRules);
	ParentPawn = Parent;
}

void ABlizzard::ApplyWizardStats()
{
	InitSpell(FVector(), ParentPawn);
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

void ABlizzard::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(ApplicationTimer);

}
