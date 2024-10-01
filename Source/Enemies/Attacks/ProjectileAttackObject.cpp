// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAttackObject.h"
#include "SpellCasting/Spells/BaseProjectile.h"

bool UProjectileAttackObject::AreAttackRequirementsMet(AActor* Target)
{
	float DistSquared = FVector::DistSquared(OwningActor->GetActorLocation(), Target->GetActorLocation());
	if (DistSquared < AttackRange * AttackRange)
	{
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, OwningActor->GetActorLocation(), Target->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel5);
		if (!Hit.bBlockingHit)
		{
			return true;
		}
	}

	return false;
}

void UProjectileAttackObject::InitiateAttack(AActor* Target)
{
	Super::InitiateAttack(Target);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.IsTimerActive(TimerHandle))
	{
		if (ChargeTime <= 0)
		{
			FireProjectile();
		}
		else
		{
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &UProjectileAttackObject::FireProjectile);
			TimerManager.SetTimer(TimerHandle, Delegate, ChargeTime, false);
		}
	}
}

void UProjectileAttackObject::InterruptAttack()
{
	Super::InterruptAttack();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UProjectileAttackObject::FireProjectile()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwningActor;
	ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(*ProjectileClass, OwningActor->GetActorLocation(), FRotator(), SpawnParams);
	Projectile->InitSpell(TargetActor->GetActorLocation(), Cast<APawn>(OwningActor));

	if (ProjectileDamageOverride > 0)
	{
		Projectile->SetBaseDamage(ProjectileDamageOverride);
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UProjectileAttackObject::CooldownCompleted);
	TimerManager.SetTimer(TimerHandle, Delegate, CooldownTime, false);
}

void UProjectileAttackObject::CooldownCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	OnAttackComponentCompletedDelegate.Broadcast(this);
}
