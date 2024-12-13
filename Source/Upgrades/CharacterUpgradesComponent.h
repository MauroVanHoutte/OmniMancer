// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpellCasting/Elements.h"
#include "CharacterUpgradesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPerkRequestSignature, WizardElement, Element);

USTRUCT(BlueprintType)
struct FUpgradesArray
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<class UCharacterUpgrade>> Upgrades;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UCharacterUpgradesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterUpgradesComponent();

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void ApplyUpgrade(class UCharacterUpgrade* Upgrade);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveUpgrade(class UCharacterUpgrade* Upgrade);
	UFUNCTION(BlueprintCallable)
	void RequestPerkSelection(WizardElement Element);

	UFUNCTION(BlueprintCallable)
	const FUpgradesArray& GetUpgradesArrayForElement(WizardElement Element);

	UPROPERTY(BlueprintAssignable)
	FPerkRequestSignature OnPerkRequestedDelegate;

private:
	UPROPERTY(Transient, VisibleAnywhere)
	TArray<class UCharacterUpgrade*> ActiveUpgrades;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<WizardElement, FUpgradesArray> AvailableLevelUpUpgrades;

	UPROPERTY(EditDefaultsOnly)
	TMap<WizardElement, class UCharacterUpgradeSet*> LevelUpUpgradeSets;
};
