// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAttackComponent.h"
#include "SpellCasting/Spells/BaseProjectile.h"

bool UProjectileAttackComponent::AreAttackRequirementsMet()
{
	float DistSquared = FVector::DistSquared(GetOwner()->GetActorLocation(), Target->GetActorLocation());
	if (DistSquared < AttackRange * AttackRange)
	{
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, GetOwner()->GetActorLocation(), Target->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel5);
		if (!Hit.bBlockingHit)
		{
			return true;
		}
	}

	return false;
}

void UProjectileAttackComponent::InitiateAttack()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.IsTimerActive(TimerHandle))
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UProjectileAttackComponent::FireProjectile);
		TimerManager.SetTimer(TimerHandle, Delegate, ChargeTime, false);
	}
}

void UProjectileAttackComponent::InterruptAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UProjectileAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
		Target = PlayerController->GetPawn();
}

void UProjectileAttackComponent::FireProjectile()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(*ProjectileClass, GetOwner()->GetActorLocation(), FRotator(), SpawnParams);
	Projectile->InitSpell(Target->GetActorLocation(), GetOwner<APawn>());

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UProjectileAttackComponent::CooldownCompleted);
	TimerManager.SetTimer(TimerHandle, Delegate, CooldownTime, false);
}

void UProjectileAttackComponent::CooldownCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	AttackCompletedDelegate.Broadcast(this);
}
