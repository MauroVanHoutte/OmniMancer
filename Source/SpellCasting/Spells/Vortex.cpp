// Fill out your copyright notice in the Description page of Project Settings.


#include "Vortex.h"
#include "Enemies/BaseCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "WizardCharacter.h"
#include <AIController.h>


AVortex::AVortex()
{
	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	RootComponent = CylinderMesh;
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder")).Object;
	CylinderMesh->SetStaticMesh(mesh);
	CylinderMesh->SetCollisionProfileName("OverlapAllDynamic");
}

void AVortex::InitSpell(const FVector& targetLocation, APawn* caster)
{
	Super::InitSpell(targetLocation, caster);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, targetLocation, targetLocation + FVector(0, 0, -500), ECollisionChannel::ECC_WorldStatic);
	SetActorLocation(Hit.bBlockingHit ? Hit.ImpactPoint : targetLocation);

	SetRadius(Radius);
}

void AVortex::SetRadius(float radius)
{
	auto scale = CylinderMesh->GetRelativeScale3D();
	scale.X = radius;
	scale.Y = radius;
	CylinderMesh->SetRelativeScale3D(scale);
}

void AVortex::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	TArray<AActor*> overlapping;
	GetOverlappingActors(overlapping, ABaseCharacter::StaticClass());
	for (AActor* actor : overlapping)
	{
		if (actor->IsA<AWizardCharacter>())
			continue;

		ABaseCharacter* character = Cast<ABaseCharacter>(actor);
		character->GetController<AAIController>()->StopMovement();
		FVector direction = (GetActorLocation() - character->GetActorLocation()).GetSafeNormal();
		character->GetCharacterMovement()->AddForce(direction * deltaTime * PullStrength);
	}
}

void AVortex::BeginPlay()
{
	Super::BeginPlay();
}

void AVortex::OnDeath()
{
	Damage = FinalDamage;
	TArray<AActor*> overlapping;
	GetOverlappingActors(overlapping, ABaseCharacter::StaticClass());

	for (AActor* actor : overlapping)
	{
		if (actor->IsA<AWizardCharacter>())
			continue;

		Cast<ABaseCharacter>(actor)->TakeSpellDamage(this);
	}
}
