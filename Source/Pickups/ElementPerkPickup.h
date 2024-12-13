// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellCasting/Elements.h"
#include "ElementPerkPickup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPerkClaimedSignature, AActor*, Player);

UCLASS()
class UNREALPROJECT_API AElementPerkPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AElementPerkPickup();

	UFUNCTION(BlueprintCallable)
	void SetElement(WizardElement NewElement);

	UPROPERTY(BlueprintAssignable)
	FPerkClaimedSignature OnPerkClaimedDelegate;

protected:
	void BeginPlay() override;

private:
	UFUNCTION()
	void OnInteractAction(AActor* Player);

	UPROPERTY(EditDefaultsOnly)
	class UInteractableComponent* InteractactableComponent;
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* ElementWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UElementTypeWidget> WidgetClass;
	UPROPERTY(EditDefaultsOnly)
	WizardElement Element = WizardElement::None;
};
