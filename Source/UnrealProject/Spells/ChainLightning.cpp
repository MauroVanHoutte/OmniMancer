// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainLightning.h"
#include <Kismet/GameplayStatics.h>
#include "../Enemies/BaseCharacter.h"
#include "../WizardCharacter.h"

AChainLightning::AChainLightning()
	: ABaseProjectile()
{
}

void AChainLightning::InitSpell(const FVector& targetLocation, APawn* caster)
{
	Super::InitSpell(targetLocation, caster);

	TotalBounces = NrOfBounces + WindLevel;
	SetActorLocation(caster->GetActorLocation());
	FireInDirection((targetLocation - caster->GetActorLocation()).GetSafeNormal());
	Damage = InitialDamage + DamagePerWindLevel * WindLevel;
	SetStunParams(true, StunDuration + DurationPerFrost * FrostLevel);
}

void AChainLightning::BeginPlay()
{
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->InitialSpeed = MaxSpeed;
	Super::BeginPlay();
}

const TArray<AActor*>& AChainLightning::GetHitActors() const
{
	return HitActors;
}

void AChainLightning::AddHitActors(const TArray<AActor*>& actors)
{
	HitActors.Append(actors);
}

void AChainLightning::OnHit(ABaseCharacter* hitActor)
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
			secondBolt->InitSpell( actors[i]->GetActorLocation(), GetOwner<APawn>()); //Split on hit
			secondBolt->FireInDirection((actors[i]->GetActorLocation() - GetActorLocation()).GetSafeNormal());
			secondBolt->AddHitActors(GetHitActors());
			secondBolt->BounceCount = BounceCount;
			auto pos = secondBolt->GetActorLocation();
			auto thisPos = GetActorLocation();
			return;
		}
	}
}

