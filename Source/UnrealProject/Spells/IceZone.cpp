// Fill out your copyright notice in the Description page of Project Settings.


#include "IceZone.h"
#include "../Enemies/BaseCharacter.h"

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

void AIceZone::SetBurnParams(bool applyBurns, float tickDamage, float tickInterval, float duration)
{
	auto effect = StatusEffects.FindByPredicate([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Damage; });

	if (applyBurns)
	{
		if (effect != nullptr)
		{
			effect->Value = tickDamage;
			effect->Interval = tickInterval;
			effect->Duration = duration;
		}
		else
		{
			StatusEffects.Add(FStatusEffect(Type::Slow, tickInterval, tickDamage, duration, this));
		}
	}
	else
	{
		if (effect != nullptr)
		{
			StatusEffects.RemoveAll([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Damage; });
		}
	}
}

void AIceZone::SetSlowParams(bool applySlow, float value, float duration)
{
	auto effect = StatusEffects.FindByPredicate([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Slow; });

	if (applySlow)
	{
		if (effect != nullptr)
		{
			effect->Value = value;
			effect->Duration = duration;
		}
		else
		{
			StatusEffects.Add(FStatusEffect(Type::Slow, -1, value, duration, this));
		}
	}
	else
	{
		if (effect != nullptr)
		{
			StatusEffects.RemoveAll([](const FStatusEffect& thisEffect) {return thisEffect.EffectType == Type::Slow; });
		}
	}
}

void AIceZone::SetRadius(float radius)
{
	auto scale = CylinderMesh->GetRelativeScale3D();
	scale.X = radius;
	scale.Y = radius;
	CylinderMesh->SetRelativeScale3D(scale);
}

void AIceZone::SetLifetime(float lifetime)
{
	GetWorld()->GetTimerManager().SetTimer(LifetimeHandle, this, &AIceZone::Destroy, lifetime);
}

void AIceZone::Destroy()
{
	AActor::Destroy();
}

// Called when the game starts or when spawned
void AIceZone::BeginPlay()
{
	Damage = 0.f;
	if (ApplyBurn)
	{
		StatusEffects.Add(FStatusEffect(Type::Damage, BurnInterval, BurnDamage, EffectLingerDuration, this));
	}
	StatusEffects.Add(FStatusEffect(Type::Slow, -1.f, SlowAmount, EffectLingerDuration, this));
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
				return;
			auto actorController = Cast<APlayerController>(baseEnemy->GetController());
			if (baseEnemy &&  instigatorController != actorController)
				baseEnemy->ReapplyStatusEffects(StatusEffects);
		}
	}

}

