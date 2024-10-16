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

bool UMovesetComponent::ExecuteAttack(AActor* Target)
{
	TArray<UBaseMove*> ValidMoves = Moves.FilterByPredicate([Target](UBaseMove* Move)
		{
			return Move->CanBeExecuted(Target);
		});

	if (ValidMoves.IsEmpty())
	{
		return false;
	}

	int RandIdx = FMath::RandRange(0, ValidMoves.Num() - 1);
	if (IsValid(ValidMoves[RandIdx]))
	{
		ActiveMove = ValidMoves[RandIdx];
		ValidMoves[RandIdx]->Execute(Target);
		return true;
	}
	return false;
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

void UMovesetComponent::OnHitTriggered(AActor* HitActor)
{
	if (IsValid(ActiveMove))
	{
		ActiveMove->OnHitTriggered(HitActor);
	}
}

bool UMovesetComponent::WasActorHitBefore(AActor* TestActor)
{
	if (IsValid(ActiveMove))
	{
		return ActiveMove->WasActorHitBefore(TestActor);
	}

	return false;
}

// Called when the game starts
void UMovesetComponent::BeginPlay()
{
	Super::BeginPlay();

	for (UBaseMove* Move : Moves)
	{ 
		Move->OnBeginPlay(GetOwner());
		Move->OnMoveCompletedDelegate.AddDynamic(this, &UMovesetComponent::OnMoveComponentCompleted);
		Move->OnMoveInterruptedDelegate.AddDynamic(this, &UMovesetComponent::OnMoveComponentInterrupted);
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
	OnAttackCompletedDelegate.Broadcast(this);
}

void UMovesetComponent::OnMoveComponentInterrupted(UBaseMove* Move)
{
	ActiveMove = nullptr;
	OnAttackInterruptedDelegate.Broadcast(this);
}