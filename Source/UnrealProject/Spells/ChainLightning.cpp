// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainLightning.h"
#include <Kismet/GameplayStatics.h>
#include "../Enemies/BaseCharacter.h"
#include "../WizardCharacter.h"

AChainLightning::AChainLightning()
	: ABaseProjectile()
{
}

void AChainLightning::InitSpell(const FVector& casterLocation, const FVector& targetLocation, const FVector& projectileDirection, AActor* owner, APawn* instigator, int fireLevel, int frostLevel, int windLevel)
{
	Super::InitSpell(casterLocation, targetLocation, projectileDirection, owner, instigator, fireLevel, frostLevel, windLevel);

	SetActorLocation(casterLocation);
	auto pos = GetActorLocation();
	FireInDirection(projectileDirection);
}

void AChainLightning::BeginPlay()
{
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->InitialSpeed = MaxSpeed;
	Super::BeginPlay();
	Damage = InitialDamage;
	if (Stuns)
	{
		StatusEffects.Add(FStatusEffect(Type::Stun, -1.f, -1.f, StunDuration, this));
	}
}

void AChainLightning::OnHit(AActor* hitActor)
{
	Super::OnHit(hitActor);

	//frost level 4+ effect
	if (FrostLevel < 4)
		return;

	TArray<AActor*> actors{};
	TArray<AActor*> ignoreActors = HitActors;
	ignoreActors.Add(hitActor);
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), BounceRange, TArray<TEnumAsByte<EObjectTypeQuery>>(), ABaseCharacter::StaticClass(), ignoreActors, actors);
	
	for (int32 i = actors.Num()-1; i > 0; i--)
	{
		if (Cast<AWizardCharacter>(actors[i]) != nullptr)
			continue;

		if (BounceCount < TotalBounces)
		{
			auto secondBolt = GetWorld()->SpawnActor<AChainLightning>(GetClass());
			secondBolt->AddHitActor(hitActor);
			secondBolt->InitSpell(GetActorLocation(), actors[i]->GetActorLocation(), (actors[i]->GetActorLocation() - GetActorLocation()).GetSafeNormal(), GetOwner(), GetInstigator(), FireLevel, FrostLevel, WindLevel);
			secondBolt->BounceCount = BounceCount;
			auto pos = secondBolt->GetActorLocation();
			auto thisPos = GetActorLocation();
			return;
		}
	}
}

