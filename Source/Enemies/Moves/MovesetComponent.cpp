// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Moves/MovesetComponent.h"
#include "Enemies/Moves/Moves.h"

// Sets default values for this component's properties
UMovesetComponent::UMovesetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UMovesetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (IsValid(ActiveMove))
	{
		ActiveMove->TickMove(DeltaTime);
	}
}

bool UMovesetComponent::CanExecuteAttack(AActor* Target)
{
	for (UBaseMove* Move : Moves)
	{
		if (Move->CanBeExecuted(Target))
			return true;
	}
	
	return false;
}

UBaseMove* UMovesetComponent::ExecuteAttack(AActor* Target)
{
	TArray<UBaseMove*> ValidMoves = Moves.FilterByPredicate([Target](UBaseMove* Move)
		{
			return Move->CanBeExecuted(Target);
		});

	if (ValidMoves.IsEmpty())
	{
		return nullptr;
	}

	if (bAvoidRepeatedMove && ValidMoves.Num() > 1)
	{
		ValidMoves.Remove(LastUsedMove);
	}

	int RandIdx = FMath::RandRange(0, ValidMoves.Num() - 1);
	if (IsValid(ValidMoves[RandIdx]))
	{
		ActiveMove = ValidMoves[RandIdx];
		ActiveMove->Execute(Target, IsValid(Target) ? Target->GetActorLocation() : FVector(0, 0, 0));
		return ActiveMove;
	}
	return nullptr;
}

bool UMovesetComponent::CanBeInterrupted()
{
	return IsValid(ActiveMove) ? ActiveMove->CanBeInterrupted() : true;
}

void UMovesetComponent::InterruptAttack()
{
	if (IsValid(ActiveMove))
	{
		ActiveMove->Interrupt();
	}
}

void UMovesetComponent::OnHitTriggered(AActor* HitActor, class UPrimitiveComponent* ColliderComponent)
{
	if (IsValid(ActiveMove))
	{
		ActiveMove->OnHitTriggered(HitActor, ColliderComponent);
	}
}

bool UMovesetComponent::WasActorHitBefore(AActor* TestActor, class UPrimitiveComponent* ColliderComponent)
{
	if (IsValid(ActiveMove))
	{
		return ActiveMove->WasActorHitBefore(TestActor, ColliderComponent);
	}

	return false;
}

// Called when the game starts
void UMovesetComponent::BeginPlay()
{
	Super::BeginPlay();

	Moves.RemoveAll([](const UBaseMove* Move) {return !IsValid(Move); });

	for (UBaseMove* Move : Moves)
	{ 
		Move->OnBeginPlay(GetOwner(), this);
		Move->OnMoveCompletedDelegate.AddDynamic(this, &UMovesetComponent::OnMoveComponentCompleted);
		Move->OnMoveHitDelegate.AddDynamic(this, &UMovesetComponent::OnMoveComponentHit);
		Move->OnMoveInterruptedDelegate.AddDynamic(this, &UMovesetComponent::OnMoveComponentInterrupted);
		Move->OnMoveExecutionStartedDelegate.AddDynamic(this, &UMovesetComponent::OnMoveExecutionStarted);
	}
}

void UMovesetComponent::EndPlay(EEndPlayReason::Type Reason)
{
	for (UBaseMove* Move : Moves)
	{
		Move->OnEndPlay();
		Move->ConditionalBeginDestroy();
	}
}

void UMovesetComponent::OnMoveComponentCompleted(UBaseMove* Move)
{
	ActiveMove = nullptr;
	LastUsedMove = Move;
	OnAttackCompletedDelegate.Broadcast(this, Move);
}

void UMovesetComponent::OnSubMoveCompleted(UBaseMove* Move)
{
	OnSubMoveCompletedDelegate.Broadcast(this, Move);
}

void UMovesetComponent::OnMoveComponentHit(UBaseMove* Move, AActor* AttackActor, AActor* HitActor)
{
	OnAttackHitDelegate.Broadcast(this, AttackActor, HitActor);
}

void UMovesetComponent::OnMoveComponentInterrupted(UBaseMove* Move)
{
	ActiveMove = nullptr;
	OnAttackInterruptedDelegate.Broadcast(this, Move);
}

void UMovesetComponent::OnMoveExecutionStarted(UBaseMove* Move)
{
	OnMoveExecutionStartedDelegate.Broadcast(this, Move);
}
