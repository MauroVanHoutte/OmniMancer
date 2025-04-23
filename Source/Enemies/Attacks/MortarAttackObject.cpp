// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/Attacks/MortarAttackObject.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "SpellCasting/Spells/SpellIndicator.h"
#include "SpellCasting/Spells/MortarProjectile.h"

void UMortarAttackObject::OnEndPlay()
{
	if (IsValid(GetWorld()))
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

bool UMortarAttackObject::AreAttackRequirementsMet(AActor* Target)
{
	float DistSquared = FVector::DistSquared(OwningActor->GetActorLocation(), Target->GetActorLocation());

	return DistSquared < AttackRange * AttackRange;
}

void UMortarAttackObject::InitiateAttack(AActor* Target, const FVector& Location)
{
	Super::InitiateAttack(Target, Location);

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
	FVector FinalTargetLocation = bUseLocationTarget ? TargetLocation : TargetActor->GetActorLocation();
	float GravityZ = GetWorld()->GetGravityZ() * 2;

	FVector OutVelocity;
	UGameplayStatics::FSuggestProjectileVelocityParameters Params{GetWorld(), OwningActor->GetActorLocation(), FinalTargetLocation, ProjectileSpeed};
	Params.bFavorHighArc = true;
	Params.TraceOption = ESuggestProjVelocityTraceOption::DoNotTrace;
	Params.OverrideGravityZ = GravityZ;
	UGameplayStatics::SuggestProjectileVelocity(Params, OutVelocity);
	
	UActorPoolingSubsystem* PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
	ASpellIndicator* Indicator = Cast<ASpellIndicator>(PoolingSystem->GetActorFromPool(IndicatorClass));
	Indicator->SetActorLocationAndRotation(FinalTargetLocation, FRotator(0, 0, 0));
	Indicator->StartCircleIndicator(FVector(IndicatorScale, IndicatorScale, 1), 2*OutVelocity.Z/-GravityZ);

	AMortarProjectile* Projectile = Cast<AMortarProjectile>(PoolingSystem->GetActorFromPool(ProjectileClass));
	Projectile->SetActorLocationAndRotation(OwningActor->GetActorLocation(), FRotator(0, 0, 0));
	Projectile->SetOwner(OwningActor);
	Projectile->SetInstigator(Cast<APawn>(OwningActor));
	UProjectileMovementComponent* ProjMovement = Projectile->GetProjectileMovementComp();
	ProjMovement->MaxSpeed = 0;
	ProjMovement->Velocity = OutVelocity;
	ProjMovement->InitialSpeed = OutVelocity.Length();
	ProjMovement->ProjectileGravityScale = 2;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UMortarAttackObject::CooldownCompleted);
	TimerManager.SetTimer(TimerHandle, Delegate, CooldownTime, false);
}

void UMortarAttackObject::CooldownCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	OnAttackCompletedDelegate.Broadcast(this);
}
