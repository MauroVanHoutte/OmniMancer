// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Attacks/DelayedProjectilesAttackObject.h"
#include "Algo/RandomShuffle.h"
#include "SpellCasting/Spells/BaseProjectile.h"

UDelayedProjectilesAttackObject::UDelayedProjectilesAttackObject()
	: Super()
	, ProjectileClass(ABaseProjectile::StaticClass())
{
}

bool UDelayedProjectilesAttackObject::AreAttackRequirementsMet(AActor* Target)
{
	return true;
}

void UDelayedProjectilesAttackObject::InitiateAttack(AActor* Target, const FVector& Location)
{
	Super::InitiateAttack(Target, Location);
	LocalProjectilePositions.Empty();

	float StartAngle = -StorageAngle / 2;
	float AngleIncrement = StorageAngle / (MaxStoredProjectiles - 1);
	
	for (int i = 0; i < MaxStoredProjectiles; i++)
	{
		float Angle = FMath::DegreesToRadians(StartAngle + AngleIncrement * i);
		FVector LocalPosition = FVector(0, FMath::Sin(Angle) * StorageRadius, FMath::Cos(Angle) * StorageRadius);
		LocalProjectilePositions.Add(LocalPosition);
	}


	if (!bChargeInOrder)
		Algo::RandomShuffle(LocalProjectilePositions);

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, bChargeSimultaneously ? &UDelayedProjectilesAttackObject::StoreAllProjectiles : &UDelayedProjectilesAttackObject::StoreProjectile);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, ChargeTime, !bChargeSimultaneously);
}

void UDelayedProjectilesAttackObject::InterruptAttack()
{
	Super::InterruptAttack();

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, bFireSimultaneously ? &UDelayedProjectilesAttackObject::FireAllProjectiles : &UDelayedProjectilesAttackObject::FireProjectile);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, FiringInterval, !bFireSimultaneously, FiringDelay);
}

void UDelayedProjectilesAttackObject::OnProjectileHit(ABaseSpell* Spell, AActor* HitActor)
{
}

void UDelayedProjectilesAttackObject::StoreProjectile()
{
	FVector LocalPosition = LocalProjectilePositions.Pop();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwningActor;
	ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(*ProjectileClass, LocalPosition, FRotator(), SpawnParams);
	StoredProjectiles.Add(Projectile);
	Projectile->AttachToActor(OwningActor, FAttachmentTransformRules::KeepRelativeTransform);
	Projectile->SetActorEnableCollision(false);

	if (LocalProjectilePositions.IsEmpty())
	{
		if (!bFireInOrder)
			Algo::RandomShuffle(StoredProjectiles);
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, bFireSimultaneously ? &UDelayedProjectilesAttackObject::FireAllProjectiles : &UDelayedProjectilesAttackObject::FireProjectile);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, FiringInterval, !bFireSimultaneously, FiringDelay);
		OnAttackCompletedDelegate.Broadcast(this); 
	}
}

void UDelayedProjectilesAttackObject::StoreAllProjectiles()
{
	while (!LocalProjectilePositions.IsEmpty())
	{
		StoreProjectile();
	}
}

void UDelayedProjectilesAttackObject::FireProjectile()
{
	ABaseProjectile* Projectile = StoredProjectiles.Pop();
	if (IsValid(Projectile) && IsValid(TargetActor))
	{
		Projectile->SetActorEnableCollision(true);
		Projectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Projectile->InitSpell(TargetActor->GetActorLocation(), Cast<APawn>(OwningActor));
		Projectile->OnSpellHitDelegate.AddDynamic(this, &UDelayedProjectilesAttackObject::OnProjectileHit);
	}

	if (StoredProjectiles.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void UDelayedProjectilesAttackObject::FireAllProjectiles()
{
	while (!StoredProjectiles.IsEmpty())
	{
		FireProjectile();
	}
}
