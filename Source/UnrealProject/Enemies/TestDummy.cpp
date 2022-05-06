// Fill out your copyright notice in the Description page of Project Settings.


#include "TestDummy.h"
#include "../Spells/BaseProjectile.h"

// Sets default values
ATestDummy::ATestDummy()
	: ABaseCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	DummyMesh->SetStaticMesh(mesh);
	DummyMesh->SetupAttachment(RootComponent);
	DummyMesh->SetCanEverAffectNavigation(false);
}

// Called when the game starts or when spawned
void ATestDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestDummy::TakeSpellDamage(float damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage taken %f"), damage);
}

void ATestDummy::TakeTickDamage(float damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Tick damage taken %f"), damage);
}


