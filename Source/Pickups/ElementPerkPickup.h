// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BaseInteractPickup.h"
#include "SpellCasting/Elements.h"
#include "ElementPerkPickup.generated.h"

UCLASS()
class UNREALPROJECT_API AElementPerkPickup : public ABaseInteractPickup
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable)
	void SetElement(WizardElement NewElement);

protected:
	void BeginPlay() override;

private:
	void OnInteractAction_Implementation(AActor* Player) override;

	UPROPERTY(EditDefaultsOnly)
	WizardElement Element = WizardElement::None;
};
