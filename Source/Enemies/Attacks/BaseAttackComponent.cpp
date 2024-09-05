// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttackComponent.h"

// Sets default values for this component's properties
UBaseAttackComponent::UBaseAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UBaseAttackComponent::TryInterruptAttack()
{
	if (IsAttackInterruptable())
	{
		InterruptAttack();
		return true;
	}
	return false;
}

void UBaseAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBaseAttackComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
}
