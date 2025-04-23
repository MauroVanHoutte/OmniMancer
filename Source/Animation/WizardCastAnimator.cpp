// Fill out your copyright notice in the Description page of Project Settings.

#include "WizardCastAnimator.h"
#include "SpellCasting/ElementManipulationComponent.h"
#include "Animation/SpringMovementMeshComponent.h"

UWizardCastAnimator::UWizardCastAnimator()
{
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

	ElementManipulationComponent->OnBasicAttackCastStartedSignature.AddDynamic(this, &UWizardCastAnimator::OnBasicAttackCastStarted);
	ElementManipulationComponent->OnBasicAttackCastedDelegate.AddDynamic(this, &UWizardCastAnimator::OnBasicAttackCasted);
	ElementManipulationComponent->OnSpellCastedDelegate.AddDynamic(this, &UWizardCastAnimator::OnSpellCasted);
}

void UWizardCastAnimator::OnBasicAttackCastStarted(UElementManipulationComponent* SpellCastingComponent, UBasePlayerCast* CastObject)
{
}

void UWizardCastAnimator::OnBasicAttackCasted(UElementManipulationComponent* CastingComponent, UBasePlayerCast* CastObject, ABaseSpell* Spell)
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

void UWizardCastAnimator::OnSpellCasted(UElementManipulationComponent* CastingComponent, UBasePlayerCast* CastObject, ABaseSpell* Spell)
{
	FTransform SpellOriginTransform = SpellCastOriginComponent->GetComponentTransform();
	LeftHandComponent->SetMeshLocationAndRotation(SpellOriginTransform.GetLocation(), SpellOriginTransform.GetRotation().Rotator());
	RightHandComponent->SetMeshLocationAndRotation(SpellOriginTransform.GetLocation(), SpellOriginTransform.GetRotation().Rotator());
}

