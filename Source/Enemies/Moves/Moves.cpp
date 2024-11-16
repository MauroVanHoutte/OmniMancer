// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Moves/Moves.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "Enemies/Moves/MoveRequirements.h"

bool UBaseMove::CanBeExecuted(AActor* Target)
{
    return IsValid(ExtraRequirement) ? ExtraRequirement->AreRequirementsMet(OwningActor, Target) : true;
}

//UAttackMove
void UAttackMove::TickMove(float DeltaTime)
{
    AttackObject->TickAttack(DeltaTime);
}

void UAttackMove::OnBeginPlay(AActor* Owner)
{
    Super::OnBeginPlay(Owner);

    AttackObject->OnBeginPlay(Owner);
    AttackObject->OnAttackCompletedDelegate.AddDynamic(this, &UAttackMove::OnAttackComponentCompleted);
    AttackObject->OnAttackHitDelegate.AddDynamic(this, &UAttackMove::OnAttackHit);
    AttackObject->OnAttackInterruptedDelegate.AddDynamic(this, &UAttackMove::OnAttackComponentInterrupted);
}

void UAttackMove::OnEndPlay()
{
    AttackObject->OnEndPlay();
    AttackObject->ConditionalBeginDestroy();
}

void UAttackMove::Execute(AActor* Target, const FVector& TargetLocation)
{
    AttackObject->InitiateAttack(Target, TargetLocation);
}

bool UAttackMove::CanBeExecuted(AActor* Target)
{
    return Super::CanBeExecuted(Target) && (bCheckAttackRequirements ? AttackObject->AreAttackRequirementsMet(Target) : true);
}

bool UAttackMove::CanBeInterrupted()
{
    return AttackObject->IsAttackInterruptable();
}

void UAttackMove::Interrupt()
{
    AttackObject->TryInterruptAttack();
}

void UAttackMove::OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent)
{
    AttackObject->OnHitTriggered(HitActor, ColliderComponent);
}

bool UAttackMove::WasActorHitBefore(AActor* TestActor, class UPrimitiveComponent* ColliderComponent)
{
    return AttackObject->WasActorHitBefore(TestActor, ColliderComponent);
}

void UAttackMove::OnAttackComponentCompleted(UBaseAttackObject* Attack)
{
    OnMoveCompletedDelegate.Broadcast(this);
}

void UAttackMove::OnAttackHit(UBaseAttackObject* Attack, AActor* AttackActor, AActor* HitActor)
{
    OnMoveHitDelegate.Broadcast(this, AttackActor, HitActor);
}

void UAttackMove::OnAttackComponentInterrupted(UBaseAttackObject* Attack)
{
    OnMoveInterruptedDelegate.Broadcast(this);
}
//~UAttackMove

//UMoveSequence
void UMoveSequence::TickMove(float DeltaTime)
{
    if (ActiveMoveIdx >= 0)
    {
        MoveSequence[ActiveMoveIdx]->TickMove(DeltaTime);
    }
}

void UMoveSequence::OnBeginPlay(AActor* Owner)
{
    Super::OnBeginPlay(Owner);

    for (UBaseMove* Move : MoveSequence)
    {
        Move->OnBeginPlay(Owner);
        Move->OnMoveHitDelegate.AddDynamic(this, &UMoveSequence::OnMoveHit);
    }
}

void UMoveSequence::OnEndPlay()
{
    for (UBaseMove* Move : MoveSequence)
    {
        Move->OnEndPlay();
        Move->ConditionalBeginDestroy();
    }
}

void UMoveSequence::Execute(AActor* Target, const FVector& TargetLocation)
{
    if (ActiveMoveIdx < 0 && !MoveSequence.IsEmpty())
    {
        TargetActor = Target;
        Location = TargetLocation;
        ActiveMoveIdx = 0;
        MoveSequence[ActiveMoveIdx]->OnMoveCompletedDelegate.AddDynamic(this, &UMoveSequence::OnMoveComponentCompleted);
        MoveSequence[ActiveMoveIdx]->Execute(TargetActor, Location);
    }
}

bool UMoveSequence::CanBeExecuted(AActor* Target)
{
    return Super::CanBeExecuted(Target) && (bCheckFirstMoveRequirements ? MoveSequence[0]->CanBeExecuted(Target) : true);
}

bool UMoveSequence::CanBeInterrupted()
{
    return MoveSequence[ActiveMoveIdx]->CanBeInterrupted();
}

void UMoveSequence::Interrupt()
{
    MoveSequence[ActiveMoveIdx]->Interrupt();
    MoveSequence[ActiveMoveIdx]->OnMoveCompletedDelegate.RemoveDynamic(this, &UMoveSequence::OnMoveComponentCompleted);
    ActiveMoveIdx = -1;
    OnMoveInterruptedDelegate.Broadcast(this);
}

void UMoveSequence::OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent)
{
    MoveSequence[ActiveMoveIdx]->OnHitTriggered(HitActor, ColliderComponent);
}

bool UMoveSequence::WasActorHitBefore(AActor* TestActor, class UPrimitiveComponent* ColliderComponent)
{
    return MoveSequence[ActiveMoveIdx]->WasActorHitBefore(TestActor, ColliderComponent);
}

void UMoveSequence::OnMoveComponentCompleted(UBaseMove* Move)
{
    MoveSequence[ActiveMoveIdx]->OnMoveCompletedDelegate.RemoveDynamic(this, &UMoveSequence::OnMoveComponentCompleted);
    ActiveMoveIdx++;
    if (ActiveMoveIdx < MoveSequence.Num())
    {
        if (!bCheckSubsequentMoveRequirements || MoveSequence[ActiveMoveIdx]->CanBeExecuted(TargetActor))
        {
            MoveSequence[ActiveMoveIdx]->OnMoveCompletedDelegate.AddDynamic(this, &UMoveSequence::OnMoveComponentCompleted);
            MoveSequence[ActiveMoveIdx]->Execute(TargetActor, Location);
            return;
        }
       
    }

    ActiveMoveIdx = -1;
    OnMoveCompletedDelegate.Broadcast(this);
}
void UMoveSequence::OnMoveHit(UBaseMove* Move, AActor* AttackActor, AActor* HitActor)
{
    OnMoveHitDelegate.Broadcast(Move, AttackActor, HitActor);
}
//~UMoveSequence

//UParallelMoves
void UParallelMoves::TickMove(float DeltaTime)
{
    for (UBaseMove* Move : Moves)
    {
        Move->TickMove(DeltaTime);
    }
}

void UParallelMoves::OnBeginPlay(AActor* Owner)
{
    Super::OnBeginPlay(Owner);

    for (UBaseMove* Move : Moves)
    {
        Move->OnBeginPlay(Owner);
        Move->OnMoveHitDelegate.AddDynamic(this, &UParallelMoves::OnMoveHit);
    }
}

void UParallelMoves::OnEndPlay()
{
    for (UBaseMove* Move : Moves)
    {
        Move->OnEndPlay();
        Move->ConditionalBeginDestroy();
    }
}

void UParallelMoves::Execute(AActor* Target, const FVector& TargetLocation)
{
    ActiveMoves = Moves;
    for (UBaseMove* Move : ActiveMoves)
    {
        Move->Execute(Target, TargetLocation);
        Move->OnMoveCompletedDelegate.AddDynamic(this, &UParallelMoves::OnMoveComponentCompleted);
    }
}

bool UParallelMoves::CanBeExecuted(AActor* Target)
{
    return Super::CanBeExecuted(Target);
}

bool UParallelMoves::CanBeInterrupted()
{
    return bCanBeInterrupted;
}

void UParallelMoves::Interrupt()
{
    for (UBaseMove* Move : ActiveMoves)
    {
        Move->Interrupt();
    }
}

void UParallelMoves::OnMoveComponentCompleted(UBaseMove* Move)
{
    Move->OnMoveCompletedDelegate.RemoveDynamic(this, &UParallelMoves::OnMoveComponentCompleted);
    ActiveMoves.Remove(Move);
    
    if (ActiveMoves.IsEmpty())
    {
        OnMoveCompletedDelegate.Broadcast(this);
    }
}

void UParallelMoves::OnMoveHit(UBaseMove* Move, AActor* AttackActor, AActor* HitActor)
{
    OnMoveHitDelegate.Broadcast(Move, AttackActor, HitActor);
}
//~UParallelMoves
