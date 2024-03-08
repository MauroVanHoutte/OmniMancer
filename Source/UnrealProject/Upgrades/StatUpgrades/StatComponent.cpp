// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatComponent::Initialize(UCharacterMovementComponent* MovementComponent)
{
	if (ensure(IsValid(MovementComponent)))
	{
		CharacterMovementComponent = MovementComponent;
		DefaultMovementSpeed = CharacterMovementComponent->MaxWalkSpeed;
	}
}

void UStatComponent::SetBaseAttackSpread(int newSpread)
{
	BaseAttackSpread = newSpread;
}

int UStatComponent::GetBaseAttackSpread()
{
	return BaseAttackSpread;
}

void UStatComponent::SetBaseAttackBounces(int newBounces)
{
	BaseAttackBounces = newBounces;
}

int UStatComponent::GetBaseAttackBounces()
{
	return BaseAttackBounces;
}

void UStatComponent::SetBaseAttackCooldownMultiplier(float newMultiplier)
{
	BaseAttackCooldownMultiplier = newMultiplier;
}

float UStatComponent::GetBaseAttackCooldownMultiplier()
{
	return BaseAttackCooldownMultiplier;
}

void UStatComponent::SetBaseAttackDamageMultiplier(float newMultiplier)
{
	BaseAttackDamageMultiplier = newMultiplier;
}

float UStatComponent::GetBaseAttackDamageMultiplier()
{
	return BaseAttackDamageMultiplier;
}

void UStatComponent::SetSpellCooldownMultiplier(float newSpellCooldownMultiplier)
{
	SpellCooldownMultiplier = newSpellCooldownMultiplier;
}

float UStatComponent::GetSpellCooldownMultiplier()
{
	return SpellCooldownMultiplier;
}

void UStatComponent::SetSpellDamageMultiplier(float newSpellDamageMultiplier)
{
	SpellDamageMultiplier = newSpellDamageMultiplier;
}

float UStatComponent::GetSpellDamageMultiplier()
{
	return SpellDamageMultiplier;
}

void UStatComponent::SetSpeedMultiplier(float newSpeedMultiplier)
{
	MovementSpeedMultiplier = newSpeedMultiplier;
	CharacterMovementComponent->MaxWalkSpeed = DefaultMovementSpeed * MovementSpeedMultiplier;
}

float UStatComponent::GetSpeedMultiplier()
{
	return MovementSpeedMultiplier;
}

void UStatComponent::SetDashForceMultiplier(float newDashForceMultiplier)
{
	DashForceMultiplier = newDashForceMultiplier;
}

float UStatComponent::GetDashForceMultiplier()
{
	return DashForceMultiplier;
}

void UStatComponent::SetDashCooldownMultiplier(float newDashCooldownMultiplier)
{
	DashCooldownMultiplier = newDashCooldownMultiplier;
}

float UStatComponent::GetDashCooldownMultiplier()
{
	return DashCooldownMultiplier;
}


