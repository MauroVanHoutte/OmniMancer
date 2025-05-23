// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAttackObject.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include "SpellCasting/Spells/BaseProjectile.h"

void UProjectileAttackObject::OnEndPlay()
{
	if (IsValid(GetWorld()))
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

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

void UProjectileAttackObject::InitiateAttack(AActor* Target, const FVector& Location)
{
	Super::InitiateAttack(Target, Location);

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
	UActorPoolingSubsystem* PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
	ABaseProjectile* Projectile = Cast<ABaseProjectile>(PoolingSystem->GetActorFromPool(ProjectileClass));
	Projectile->SetOwner(OwningActor);
	Projectile->SetInstigator(Cast<APawn>(OwningActor));
	Projectile->SetActorLocationAndRotation(OwningActor->GetActorLocation(), FRotator(0, 0, 0));
	Projectile->InitSpell(bUseLocationTarget ? TargetLocation : (IsValid(TargetActor) ? TargetActor->GetActorLocation() : FVector(0, 0, 0)), Cast<APawn>(OwningActor));
	Projectile->OnSpellHitDelegate.AddDynamic(this, &UProjectileAttackObject::OnProjectileHit);

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
	OnAttackCompletedDelegate.Broadcast(this);
}

void UProjectileAttackObject::OnProjectileHit(ABaseSpell* Spell, AActor* HitActor)
{
	OnAttackHitDelegate.Broadcast(this, Spell, HitActor);
}
