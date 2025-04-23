// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttackObject.h"

void UBaseAttackObject::OnBeginPlay(AActor* Owner)
{
	OwningActor = Owner;
}

bool UBaseAttackObject::TryInterruptAttack()
{
	if (IsAttackInterruptable())
	{
		InterruptAttack();
		return true;
	}
	return false;
}

bool UBaseAttackObject::CompareTag(const FName& Tag)
{
	return Tag != NAME_None && AttackTag == Tag;
}
