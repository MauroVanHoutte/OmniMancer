// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Elements.h"
#include "SpellCasting/Spells/BaseSpell.h"
#include "Templates/Tuple.h"
#include "ElementManipulationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FElementAddedSignature, WizardElement, OldElement, WizardElement, NewElement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellCastedSignature, AActor*, Caster, ABaseSpell*, Spell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTriggeredSpellCastedSignature, AActor*, Caster, ABaseSpell*, Spell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBasicAttackCastedSignature, AActor*, Caster, ABaseSpell*, Spell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellHitSignature, ABaseSpell*, Spell, AActor*, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBasicAttackHitSignature, ABaseSpell*, Spell, AActor*, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellDestroyedSignature, ABaseSpell*, Spell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FElementLearnedSignature, WizardElement, LearnedElement);

USTRUCT(BlueprintType)
struct UNREALPROJECT_API FSpellConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<WizardElement> ElementCombination;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABaseSpell> Spell;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* SpellIcon;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UElementManipulationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UElementManipulationComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(class APlayerController* Controller, class UNiagaraComponent* LeftParticle, class UNiagaraComponent* RightParticle, class UStatComponent* StatComponent);

	UFUNCTION(BlueprintCallable)
	void AddElement(WizardElement Element);

	UFUNCTION(BlueprintCallable)
	void TryCastSpell();

	UFUNCTION(BlueprintCallable)
	void TryCastBasicAttack();

	UFUNCTION(BlueprintCallable)
	class ABaseSpell* TriggeredCast(TSubclassOf<ABaseSpell> SpellClass, const FVector& TargetLocation, const FVector& SpawnLocation, bool SendSpellEvents);

	UFUNCTION(BlueprintCallable)
	const TSet<WizardElement>& GetLearnedElements() const;

	UFUNCTION(BlueprintCallable)
	const TArray<WizardElement>& GetActiveElements() const;

	UFUNCTION(BlueprintCallable)
	void LearnElement(WizardElement Element);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const TArray<FSpellConfig>& GetSpellConfig();

	UFUNCTION(BlueprintCallable)
	void AddCooldownMultiplier(TSubclassOf<ABaseSpell> ApplicableSpell, float CooldownMultiplier);
	UFUNCTION(BlueprintCallable)
	void AddCooldownMultiplierForElement(WizardElement Element, float CooldownMultiplier);

	UFUNCTION(BlueprintCallable)
	TMap<TSubclassOf<class ABaseSpell>, float>& GetSpellCooldowns();
	UFUNCTION(BlueprintCallable)
	TMap<TSubclassOf<class ABaseSpell>, FTimerHandle>& GetSpellCooldownTimers();

	class USceneComponent* GetBasicAttackOrigin() const;
	void SetBasicAttackOrigin(class USceneComponent* NewBasicAttackOrigin);

	UPROPERTY(BlueprintAssignable)
	FElementAddedSignature OnElementAddedDelegate;
	UPROPERTY(BlueprintAssignable)
	FSpellCastedSignature OnSpellCastedDelegate;
	UPROPERTY(BlueprintAssignable)
	FTriggeredSpellCastedSignature OnTriggeredSpellCastedDelegate;
	UPROPERTY(BlueprintAssignable)
	FBasicAttackCastedSignature OnBasicAttackCastedDelegate;
	UPROPERTY(BlueprintAssignable)
	FSpellHitSignature OnSpellHitDelegate;
	UPROPERTY(BlueprintAssignable)
	FBasicAttackHitSignature OnBasicAttackHitDelegate;
	UPROPERTY(BlueprintAssignable)
	FElementLearnedSignature OnElementLearnedDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void RaycastMouseOnLevel(FVector& mouseAtWizardHeight, FHitResult& raycastHit);
	void SetupSpells();

	UFUNCTION()
	void OnSpellHit(ABaseSpell* Spell, AActor* HitActor);
	UFUNCTION()
	void OnBasicAttackHit(ABaseSpell* Spell, AActor* HitActor);
	float CalculateSpellCooldown(TSubclassOf<class ABaseSpell> Spell);

	UPROPERTY(EditDefaultsOnly)
	TSet<WizardElement> LearnedElements { WizardElement::Fire };

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpellConfig> SpellConfiguration;
	UPROPERTY(Transient)
	TMap<int, TSubclassOf<class ABaseSpell>> Spells;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseSpell> BasicAttack;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<class ABaseSpell>, float> Cooldowns{};
	UPROPERTY(VisibleAnywhere)
	TMap<TSubclassOf<class ABaseSpell>, FTimerHandle> CooldownTimers{};
	UPROPERTY(VisibleAnywhere)
	TMap<TSubclassOf<class ABaseSpell>, float> SpellCooldownMultipliers{};
	UPROPERTY(VisibleAnywhere)
	TMap<WizardElement, float> ElementCooldownMultipliers{};

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* BasicAttackOriginPoint;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* SpellOriginPoint;

	UPROPERTY(Transient)
	class APlayerController* Controller;
	UPROPERTY(Transient)
	class UStatComponent* Stats;

	UPROPERTY(EditDefaultsOnly)
	TArray<WizardElement> CurrentElements;
	UPROPERTY(EditDefaultsOnly)
	TMap<WizardElement, FLinearColor> ElementColors;
	UPROPERTY(EditDefaultsOnly)
	FName ColorVariableName;
	UPROPERTY(Transient, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UNiagaraComponent* LeftHandElementParticle;
	UPROPERTY(Transient, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UNiagaraComponent* RightHandElementParticle;
	bool bAddingElementLeft;
};
