// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/Attacks/MortarAttackObject.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "SpellCasting/Spells/SpellIndicator.h"
#include "SpellCasting/Spells/MortarProjectile.h"

void UMortarAttackObject::OnEndPlay()
{
	OwningActor->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

bool UMortarAttackObject::AreAttackRequirementsMet(AActor* Target)
{
	float DistSquared = FVector::DistSquared(OwningActor->GetActorLocation(), Target->GetActorLocation());

	return DistSquared < AttackRange * AttackRange;
}

void UMortarAttackObject::InitiateAttack(AActor* Target)
{
	Super::InitiateAttack(Target);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.IsTimerActive(TimerHandle))
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UMortarAttackObject::FireProjectile);
		TimerManager.SetTimer(TimerHandle, Delegate, ChargeTime, false);
	}
}

void UMortarAttackObject::InterruptAttack()
{
	Super::InterruptAttack();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UMortarAttackObject::FireProjectile()
{
	FVector OutVelocity;
	UGameplayStatics::FSuggestProjectileVelocityParameters Params{GetWorld(), OwningActor->GetActorLocation(), TargetActor->GetActorLocation(), ProjectileSpeed};
	Params.bFavorHighArc = true;
	Params.TraceOption = ESuggestProjVelocityTraceOption::DoNotTrace;
	UGameplayStatics::SuggestProjectileVelocity(Params, OutVelocity);

	float GravityZ = GetWorld()->GetGravityZ();
	
	ASpellIndicator* Indicator = GetWorld()->SpawnActor<ASpellIndicator>(IndicatorClass, TargetActor->GetActorLocation(), FRotator(0));
	Indicator->StartCircleIndicator(FVector(IndicatorScale, IndicatorScale, 1), 2*OutVelocity.Z/-GravityZ);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwningActor;
	SpawnParams.Instigator = Cast<APawn>(OwningActor);
	AMortarProjectile* Projectile = GetWorld()->SpawnActor<AMortarProjectile>(*ProjectileClass, OwningActor->GetActorLocation(), FRotator(0), SpawnParams);
	UProjectileMovementComponent* ProjMovement = Projectile->GetProjectileMovementComp();
	ProjMovement->MaxSpeed = 0;
	ProjMovement->Velocity = OutVelocity;
	ProjMovement->InitialSpeed = OutVelocity.Length();

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UMortarAttackObject::CooldownCompleted);
	TimerManager.SetTimer(TimerHandle, Delegate, CooldownTime, false);
}

void UMortarAttackObject::CooldownCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	OnAttackComponentCompletedDelegate.Broadcast(this);
}
