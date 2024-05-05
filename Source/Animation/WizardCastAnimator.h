// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WizardCastAnimator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UWizardCastAnimator : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWizardCastAnimator();
	
	UFUNCTION(BlueprintCallable)
	void Initialize(class UElementManipulationComponent* ElementManipulation, 
		class USpringMovementMeshComponent* LeftHand, 
		class USpringMovementMeshComponent* RightHand, 
		class USceneComponent* LeftAttackOrigin,
		class USceneComponent* RightAttackOrigin,
		class USceneComponent* SpellCastOrigin);

private:
	UFUNCTION()
	void OnBasicAttackCasted(AActor* Caster, class ABaseSpell* Spell);

	UFUNCTION()
	void OnSpellCasted(AActor* Caster, class ABaseSpell* Spell);

	class UElementManipulationComponent* ElementManipulationComponent;
	class USpringMovementMeshComponent* LeftHandComponent;
	class USpringMovementMeshComponent* RightHandComponent;
	class USceneComponent* LeftAttackOriginComponent;
	class USceneComponent* RightAttackOriginComponent;
	class USceneComponent* SpellCastOriginComponent;
};
