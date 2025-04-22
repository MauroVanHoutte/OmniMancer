// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Moves/Moves.h"
#include "Enemies/Attacks/BaseAttackObject.h"
#include "Enemies/Moves/MoveRequirements.h"
#include "Enemies/Moves/MovesetComponent.h"

void UBaseMove::Execute(AActor* Target, const FVector& TargetLocation)
{
    if (ExtraRequirements.Num() > 0)
    {
        for (UBaseMoveRequirement* ExtraRequirement : ExtraRequirements)
        {
            if (IsValid(ExtraRequirement))
            {
                ExtraRequirement->OnAttackExecuted(OwningActor, Target);
            }
        }
    }

    OnMoveExecutionStartedDelegate.Broadcast(this);
}

bool UBaseMove::CanBeExecuted(AActor* Target)
{
    bool bExtraRequirementsMet = true;
    for (UBaseMoveRequirement* ExtraRequirement : ExtraRequirements)
    {
        if (IsValid(ExtraRequirement))
        {
            if (!ExtraRequirement->AreRequirementsMet(OwningActor, Target))
            {
                return false;
            }
        }
    }
    return true;
}

//UAttackMove
void UAttackMove::TickMove(float DeltaTime)
{
    AttackObject->TickAttack(DeltaTime);
}

void UAttackMove::OnBeginPlay(AActor* Owner, UMovesetComponent* MovesetComponent)
{
    Super::OnBeginPlay(Owner, MovesetComponent);

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

UBaseAttackObject* UAttackMove::GetAttackObject()
{
    return AttackObject;
}

void UAttackMove::Execute(AActor* Target, const FVector& TargetLocation)
{
    Super::Execute(Target, TargetLocation);
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
    if (!AttackObject->WasActorHitBefore(HitActor, ColliderComponent))
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

void UMoveSequence::OnBeginPlay(AActor* Owner, UMovesetComponent* MovesetComponent)
{
    Super::OnBeginPlay(Owner, MovesetComponent);

    for (UBaseMove* Move : MoveSequence)
    {
        Move->OnBeginPlay(Owner, MovesetComponent);
        Move->OnMoveHitDelegate.AddDynamic(this, &UMoveSequence::OnMoveHit);
        Move->OnMoveExecutionStartedDelegate.AddDynamic(this, &UMoveSequence::OnMoveExecutionStarted);
        Move->OnMoveCompletedDelegate.AddDynamic(MovesetComponent, &UMovesetComponent::OnSubMoveCompleted);
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
    Super::Execute(Target, TargetLocation);

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
    return ActiveMoveIdx < 0 ? true : MoveSequence[ActiveMoveIdx]->CanBeInterrupted();
}

void UMoveSequence::Interrupt()
{
    if (ActiveMoveIdx >= 0)
    {
        MoveSequence[ActiveMoveIdx]->Interrupt();
        MoveSequence[ActiveMoveIdx]->OnMoveCompletedDelegate.RemoveDynamic(this, &UMoveSequence::OnMoveComponentCompleted);
        ActiveMoveIdx = -1;
        OnMoveInterruptedDelegate.Broadcast(this);
    }
}

void UMoveSequence::OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent)
{
    if (ActiveMoveIdx >= 0 && ActiveMoveIdx < MoveSequence.Num())
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
    if (MoveCompletionDelay > 0)
    {
        FTimerManager& TimerManager = GetWorld()->GetTimerManager();
        FTimerDelegate Delegate;
        Delegate.BindUObject(this, &UMoveSequence::BroadcastMoveCompleted);
        TimerManager.SetTimer(CompletionDelayHandle, Delegate, MoveCompletionDelay, false);
    }
    else
    {
        BroadcastMoveCompleted();
    }
}

void UMoveSequence::BroadcastMoveCompleted()
{
    OnMoveCompletedDelegate.Broadcast(this);
}

void UMoveSequence::OnMoveHit(UBaseMove* Move, AActor* AttackActor, AActor* HitActor)
{
    OnMoveHitDelegate.Broadcast(Move, AttackActor, HitActor);
}
void UMoveSequence::OnMoveExecutionStarted(UBaseMove* Move)
{
    OnMoveExecutionStartedDelegate.Broadcast(Move);
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

void UParallelMoves::OnBeginPlay(AActor* Owner, UMovesetComponent* MovesetComponent)
{
    Super::OnBeginPlay(Owner, MovesetComponent);

    for (UBaseMove* Move : Moves)
    {
        Move->OnBeginPlay(Owner, MovesetComponent);
        Move->OnMoveHitDelegate.AddDynamic(this, &UParallelMoves::OnMoveHit);
        Move->OnMoveExecutionStartedDelegate.AddDynamic(this, &UParallelMoves::OnMoveExecutionStarted);
        Move->OnMoveCompletedDelegate.AddDynamic(MovesetComponent, &UMovesetComponent::OnSubMoveCompleted);
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
    Super::Execute(Target, TargetLocation);

    ActiveMoves = Moves;
    for (UBaseMove* Move : ActiveMoves)
    {
        Move->OnMoveCompletedDelegate.AddDynamic(this, &UParallelMoves::OnMoveComponentCompleted);
        Move->Execute(Target, TargetLocation);
    }
}

bool UParallelMoves::CanBeExecuted(AActor* Target)
{
    for (UBaseMove* Move : Moves)
    {
        if (!Move->CanBeExecuted(Target))
            return false;
    }

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

void UParallelMoves::OnHitTriggered(AActor* HitActor, UPrimitiveComponent* ColliderComponent)
{
    for (UBaseMove* Move : Moves)
    {
        Move->OnHitTriggered(HitActor, ColliderComponent);
    }
}

bool UParallelMoves::WasActorHitBefore(AActor* TestActor, UPrimitiveComponent* ColliderComponent)
{
    return false;
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

void UParallelMoves::OnMoveExecutionStarted(UBaseMove* Move)
{
    OnMoveExecutionStartedDelegate.Broadcast(Move);
}
//~UParallelMoves
