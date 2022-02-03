// Fill out your copyright notice in the Description page of Project Settings.


#include "IceZone.h"
#include "../Enemies/BaseCharacter.h"

// Sets default values
AIceZone::AIceZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	RootComponent = m_CylinderMesh;
	auto mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder")).Object;
	m_CylinderMesh->SetStaticMesh(mesh);
	m_CylinderMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	m_CylinderMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	
}

// Called when the game starts or when spawned
void AIceZone::BeginPlay()
{
	InitialLifeSpan = m_Lifetime;

	m_Damage = 0.f;
	if (m_ApplyBurn)
	{
		m_StatusEffects.Add(FStatusEffect(Type::Damage, m_BurnInterval, m_BurnDamage, m_EffectLingerDuration, this));
	}
	m_StatusEffects.Add(FStatusEffect(Type::Slow, -1.f, m_SlowAmount, m_EffectLingerDuration, this));
	m_CylinderMesh->SetRelativeScale3D(FVector(m_CircleScale, m_CircleScale, 1.f));

	Super::BeginPlay();

}

// Called every frame
void AIceZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	m_ApplicationTimer += DeltaTime;
	if (m_ApplicationTimer > m_ApplicationInterval)
	{
		m_ApplicationTimer -= m_ApplicationInterval;

		TArray<AActor*> overlappingActors{};
		GetOverlappingActors(overlappingActors);

		for (auto& actor : overlappingActors)
		{
			auto instigatorController = Cast<APlayerController>(GetInstigatorController());
			auto baseEnemy = Cast<ABaseCharacter>(actor);
			if (!baseEnemy)
				return;
			auto actorController = Cast<APlayerController>(baseEnemy->GetController());
			if (baseEnemy &&  instigatorController != actorController)
				baseEnemy->ReapplyStatusEffects(m_StatusEffects);
		}
	}

}

