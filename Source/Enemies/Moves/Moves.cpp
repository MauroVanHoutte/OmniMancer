// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Moves/Moves.h"
#include "Enemies/Attacks/BaseAttackObject.h"

//UAttackMove
void UAttackMove::TickMove(float DeltaTime)
{
    AttackObject->TickAttack(DeltaTime);
}

void UAttackMove::OnBeginPlay(AActor* Owner)
{
    AttackObject->OnBeginPlay(Owner);
    AttackObject->OnAttackComponentCompletedDelegate.AddDynamic(this, &UAttackMove::OnAttackComponentCompleted);
    AttackObject->OnAttackComponentInterruptedDelegate.AddDynamic(this, &UAttackMove::OnAttackComponentInterrupted);
}

void UAttackMove::Execute(AActor* Target)
{
    AttackObject->InitiateAttack(Target);
}

bool UAttackMove::CanBeExecuted(AActor* Target)
{
    return bCheckAttackRequirements ? AttackObject->AreAttackRequirementsMet(Target) : true;
}

bool UAttackMove::CanBeInterrupted()
{
    return AttackObject->IsAttackInterruptable();
}

void UAttackMove::Interrupt()
{
    AttackObject->TryInterruptAttack();
}

void UAttackMove::OnHitTriggered(AActor* HitActor)
{
    AttackObject->OnHitTriggered(HitActor);
}

bool UAttackMove::WasActorHitBefore(AActor* TestActor)
{
    return AttackObject->WasActorHitBefore(TestActor);
}

void UAttackMove::OnAttackComponentCompleted(UBaseAttackObject* Attack)
{
    OnMoveCompletedDelegate.Broadcast(this);
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
    for (UBaseMove* Move : MoveSequence)
    {
        Move->OnBeginPlay(Owner);
    }
}

void UMoveSequence::Execute(AActor* Target)
{
    if (ActiveMoveIdx < 0 && !MoveSequence.IsEmpty())
    {
        TargetActor = Target;
        ActiveMoveIdx = 0;
        MoveSequence[ActiveMoveIdx]->OnMoveCompletedDelegate.AddDynamic(this, &UMoveSequence::OnMoveComponentCompleted);
        MoveSequence[ActiveMoveIdx]->OnMoveInterruptedDelegate.AddDynamic(this, &UMoveSequence::OnMoveComponentInterrupted);
        MoveSequence[ActiveMoveIdx]->Execute(TargetActor);
    }
}

bool UMoveSequence::CanBeExecuted(AActor* Target)
{
    return bCheckFirstMoveRequirements ? MoveSequence[0]->CanBeExecuted(Target) : true;
}

bool UMoveSequence::CanBeInterrupted()
{
    return MoveSequence[ActiveMoveIdx]->CanBeInterrupted();
}

void UMoveSequence::Interrupt()
{
    MoveSequence[ActiveMoveIdx]->Interrupt();
}

void UMoveSequence::OnHitTriggered(AActor* HitActor)
{
    MoveSequence[ActiveMoveIdx]->OnHitTriggered(HitActor);
}

bool UMoveSequence::WasActorHitBefore(AActor* TestActor)
{
    return MoveSequence[ActiveMoveIdx]->WasActorHitBefore(TestActor);
}

void UMoveSequence::OnMoveComponentCompleted(UBaseMove* Move)
{
    MoveSequence[ActiveMoveIdx]->OnMoveCompletedDelegate.RemoveDynamic(this, &UMoveSequence::OnMoveComponentCompleted);
    MoveSequence[ActiveMoveIdx]->OnMoveInterruptedDelegate.RemoveDynamic(this, &UMoveSequence::OnMoveComponentInterrupted);
    ActiveMoveIdx++;
    if (ActiveMoveIdx < MoveSequence.Num())
    {
        if (!bCheckSubsequentMoveRequirements || MoveSequence[ActiveMoveIdx]->CanBeExecuted(TargetActor))
        {
            MoveSequence[ActiveMoveIdx]->OnMoveCompletedDelegate.AddDynamic(this, &UMoveSequence::OnMoveComponentCompleted);
            MoveSequence[ActiveMoveIdx]->OnMoveInterruptedDelegate.AddDynamic(this, &UMoveSequence::OnMoveComponentInterrupted);
            MoveSequence[ActiveMoveIdx]->Execute(TargetActor);
            return;
        }
       
    }

    ActiveMoveIdx = -1;
    OnMoveCompletedDelegate.Broadcast(this);
}

void UMoveSequence::OnMoveComponentInterrupted(UBaseMove* Move)
{
    MoveSequence[ActiveMoveIdx]->OnMoveCompletedDelegate.RemoveDynamic(this, &UMoveSequence::OnMoveComponentCompleted);
    MoveSequence[ActiveMoveIdx]->OnMoveInterruptedDelegate.RemoveDynamic(this, &UMoveSequence::OnMoveComponentInterrupted);
    ActiveMoveIdx = -1;
    OnMoveInterruptedDelegate.Broadcast(this);
}
//~UMoveSequence