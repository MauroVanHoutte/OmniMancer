// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/Attacks/MortarAttackComponent.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "SpellCasting/Spells/SpellIndicator.h"
#include "SpellCasting/Spells/MortarProjectile.h"

bool UMortarAttackComponent::AreAttackRequirementsMet()
{
	float DistSquared = FVector::DistSquared(GetOwner()->GetActorLocation(), Target->GetActorLocation());

	return DistSquared < AttackRange * AttackRange;
}

void UMortarAttackComponent::InitiateAttack()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.IsTimerActive(TimerHandle))
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UMortarAttackComponent::FireProjectile);
		TimerManager.SetTimer(TimerHandle, Delegate, ChargeTime, false);
	}
}

void UMortarAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
		Target = PlayerController->GetPawn();
}

void UMortarAttackComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
}

void UMortarAttackComponent::InterruptAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UMortarAttackComponent::FireProjectile()
{
	FVector OutVelocity;
	UGameplayStatics::SuggestProjectileVelocity(GetWorld(), OutVelocity, GetOwner()->GetActorLocation(),
		Target->GetActorLocation(), ProjectileSpeed, true, 0.f, 0.0f, ESuggestProjVelocityTraceOption::DoNotTrace);

	float GravityZ = GetWorld()->GetGravityZ();
	
	ASpellIndicator* Indicator = GetWorld()->SpawnActor<ASpellIndicator>(IndicatorClass, Target->GetActorLocation(), FRotator());
	Indicator->StartCircleIndicator(FVector(IndicatorScale, IndicatorScale, 1), 2*OutVelocity.Z/-GravityZ);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner<APawn>();
	AMortarProjectile* Projectile = GetWorld()->SpawnActor<AMortarProjectile>(*ProjectileClass, GetOwner()->GetActorLocation(), FRotator(), SpawnParams);
	UProjectileMovementComponent* ProjMovement = Projectile->GetProjectileMovementComp();
	ProjMovement->MaxSpeed = 0;
	ProjMovement->Velocity = OutVelocity;
	ProjMovement->InitialSpeed = OutVelocity.Length();

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UMortarAttackComponent::CooldownCompleted);
	TimerManager.SetTimer(TimerHandle, Delegate, CooldownTime, false);
}

void UMortarAttackComponent::CooldownCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	AttackCompletedDelegate.Broadcast(this);
}
