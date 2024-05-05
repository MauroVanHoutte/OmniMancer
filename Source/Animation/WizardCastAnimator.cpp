// Fill out your copyright notice in the Description page of Project Settings.

#include "WizardCastAnimator.h"
#include "SpellCasting/ElementManipulationComponent.h"
#include "Animation/SpringMovementMeshComponent.h"

// Sets default values for this component's properties
UWizardCastAnimator::UWizardCastAnimator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWizardCastAnimator::Initialize(UElementManipulationComponent* ElementManipulation, 
	USpringMovementMeshComponent* LeftHand, 
	USpringMovementMeshComponent* RightHand, 
	USceneComponent* LeftAttackOrigin, 
	USceneComponent* RightAttackOrigin, 
	USceneComponent* SpellCastOrigin)
{
	ElementManipulationComponent = ElementManipulation;
	LeftHandComponent = LeftHand;
	RightHandComponent = RightHand;
	LeftAttackOriginComponent = LeftAttackOrigin;
	RightAttackOriginComponent = RightAttackOrigin;
	SpellCastOriginComponent = SpellCastOrigin;

	ElementManipulationComponent->OnBasicAttackCastedDelegate.AddDynamic(this, &UWizardCastAnimator::OnBasicAttackCasted);
	ElementManipulationComponent->OnSpellCastedDelegate.AddDynamic(this, &UWizardCastAnimator::OnSpellCasted);
}

void UWizardCastAnimator::OnBasicAttackCasted(AActor* Caster, ABaseSpell* Spell)
{
	USceneComponent* CurrentAttackOrigin = ElementManipulationComponent->GetBasicAttackOrigin();
	USpringMovementMeshComponent* CurrentHand;
	USceneComponent* NextAttackOrigin;

	if (CurrentAttackOrigin == LeftAttackOriginComponent)
	{
		CurrentHand = LeftHandComponent;
		NextAttackOrigin = RightAttackOriginComponent;
	}
	else
	{
		CurrentHand = RightHandComponent;
		NextAttackOrigin = LeftAttackOriginComponent;
	}

	FTransform AttackOriginTransform = CurrentAttackOrigin->GetComponentTransform();
	CurrentHand->SetMeshLocationAndRotation(AttackOriginTransform.GetLocation(), AttackOriginTransform.GetRotation().Rotator());
	ElementManipulationComponent->SetBasicAttackOrigin(NextAttackOrigin);
}

void UWizardCastAnimator::OnSpellCasted(AActor* Caster, ABaseSpell* Spell)
{
	FTransform SpellOriginTransform = SpellCastOriginComponent->GetComponentTransform();
	LeftHandComponent->SetMeshLocationAndRotation(SpellOriginTransform.GetLocation(), SpellOriginTransform.GetRotation().Rotator());
	RightHandComponent->SetMeshLocationAndRotation(SpellOriginTransform.GetLocation(), SpellOriginTransform.GetRotation().Rotator());
}

