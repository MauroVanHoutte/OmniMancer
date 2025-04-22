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

void UStatComponent::SetDamageMultiplierForElement(WizardElement Element, float newDamageMultiplier)
{
	ElementDamageMultipliers.FindOrAdd(Element) = newDamageMultiplier;
}

float UStatComponent::GetDamageMultiplierForElement(WizardElement Element)
{
	return ElementDamageMultipliers.FindOrAdd(Element, 1);
}

void UStatComponent::SetCooldownMultiplierForElement(WizardElement Element, float newCooldownMultiplier)
{
	ElementCooldownMultipliers.FindOrAdd(Element) = newCooldownMultiplier;
}

float UStatComponent::GetCooldownMultiplierForElement(WizardElement Element)
{
	return ElementCooldownMultipliers.FindOrAdd(Element, 1);
}

void UStatComponent::SetExtraBounces(int newBounces)
{
	ExtraBounces = newBounces;
}

int UStatComponent::GetExtraBounces()
{
	return ExtraBounces;
}

void UStatComponent::SetSpeedMultiplier(float newSpeedMultiplier)
{
	MovementSpeedMultiplier = newSpeedMultiplier;
	if (IsValid(CharacterMovementComponent))
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

void UStatComponent::SetCurrency(int NewCurrency)
{
	Currency = NewCurrency;
}

int UStatComponent::GetCurrency()
{
	return Currency;
}