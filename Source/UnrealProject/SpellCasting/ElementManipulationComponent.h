// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Elements.h"
#include "../Spells/BaseSpell.h"
#include "Templates/Tuple.h"
#include "ElementManipulationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FElementAddedSignature, WizardElement, OldElement, WizardElement, NewElement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellCastedSignature, AActor*, Caster, ABaseSpell*, Spell);

USTRUCT(BlueprintType)
struct UNREALPROJECT_API FSpellConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<WizardElement> ElementCombination;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseSpell> Spell;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UElementManipulationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UElementManipulationComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(class APlayerController* Controller, class UBillboardComponent* FirstElementBillboard, class UBillboardComponent* SecondElementBillboard);

	UFUNCTION(BlueprintCallable)
	void AddElement(WizardElement Element);

	UFUNCTION(BlueprintCallable)
	void TryCastSpell();

	UPROPERTY(BlueprintAssignable)
	FElementAddedSignature OnElementAdded;
	UPROPERTY(BlueprintAssignable)
	FSpellCastedSignature OnSpellCasted;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void RaycastMouseOnLevel(FVector& mouseAtWizardHeight, FHitResult& raycastHit);
	void SetupSpells();

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpellConfig> SpellConfiguration;
	UPROPERTY()
	TMap<int, TSubclassOf<class ABaseSpell>> Spells;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<class ABaseSpell>, float> Cooldowns{};
	UPROPERTY(VisibleAnywhere)
	TMap<TSubclassOf<class ABaseSpell>, FTimerHandle> CooldownTimers{};

	UPROPERTY(Transient, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class APlayerController* Controller;

	UPROPERTY(EditDefaultsOnly)
	TArray<WizardElement> CurrentElements;
	UPROPERTY(EditDefaultsOnly)
	TMap<WizardElement, class UTexture2D*> ElementTextures;
	UPROPERTY(Transient, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UBillboardComponent* FirstElementBillboard;
	UPROPERTY(Transient, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UBillboardComponent* SecondElementBillboard;
};
