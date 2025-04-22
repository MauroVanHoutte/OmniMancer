// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Elements.h"
#include "SpellCasting/Spells/BaseSpell.h"
#include "Templates/Tuple.h"
#include "ElementManipulationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FElementAddedSignature, UElementManipulationComponent*, SpellcastingComponent, WizardElement, OldElement, WizardElement, NewElement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellCastStartedSignature, UElementManipulationComponent*, SpellcastingComponent, class UBasePlayerCast*, CastObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBasicAttackCastStartedSignature, UElementManipulationComponent*, SpellCastingComponent, class UBasePlayerCast*, CastObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSpellCastedSignature, UElementManipulationComponent*, SpellcastingComponent, class UBasePlayerCast*, CastObject, class ABaseSpell*, Spell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTriggeredSpellCastedSignature, UElementManipulationComponent*, SpellcastingComponent, class ABaseSpell*, Spell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBasicAttackCastedSignature, UElementManipulationComponent*, SpellcastingComponent, class UBasePlayerCast*, CastObject, class ABaseSpell*, Spell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellHitSignature, UElementManipulationComponent*, SpellcastingComponent, class UBasePlayerCast*, CastObject, class ABaseSpell*, Spell, AActor*, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBasicAttackHitSignature, UElementManipulationComponent*, SpellcastingComponent, class UBasePlayerCast*, CastObject, class ABaseSpell*, Spell, AActor*, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellDestroyedSignature, class ABaseSpell*, Spell);
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

USTRUCT(BlueprintType)
struct UNREALPROJECT_API FSpellCastConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<WizardElement> ElementCombination;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	class UBasePlayerCast* CastObject;
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
	void HandleCastInputPressed();
	UFUNCTION(BlueprintCallable)
	void HandleCastInputDown();
	UFUNCTION(BlueprintCallable)
	void HandleCastInputReleased();

	UFUNCTION(BlueprintCallable)
	void TryCastBasicAttack();
	UFUNCTION(BlueprintCallable)
	void HandleBasicAttackInputPressed();
	UFUNCTION(BlueprintCallable)
	void HandleBasicAttackInputDown();
	UFUNCTION(BlueprintCallable)
	void HandleBasicAttackInputReleased();

	UFUNCTION(BlueprintCallable)
	class ABaseSpell* TriggeredCast(TSubclassOf<ABaseSpell> SpellClass, const FVector& TargetLocation, const FVector& SpawnLocation, bool SendSpellEvents);

	UFUNCTION(BlueprintCallable)
	const TSet<WizardElement>& GetLearnedElements() const;

	UFUNCTION(BlueprintCallable)
	const TArray<WizardElement>& GetActiveElements() const;

	UFUNCTION(BlueprintCallable)
	void LearnElement(WizardElement Element);
	UFUNCTION(BlueprintCallable)
	void UnlearnElement(WizardElement Element);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const TArray<FSpellConfig>& GetSpellConfigs();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FSpellCastConfig>& GetSpellCastConfigs();

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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSpellCastCooldownMultiplier(class UBasePlayerCast* SpellCast);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSpellCastDamageMultiplier(class UBasePlayerCast* SpellCast);

	UPROPERTY(BlueprintAssignable)
	FElementAddedSignature OnElementAddedDelegate;
	UPROPERTY(BlueprintAssignable)
	FBasicAttackCastStartedSignature OnBasicAttackCastStartedSignature;
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
	void OnSpellCastStarted(class UBasePlayerCast* CastObject);
	UFUNCTION()
	void OnSpellCasted(class UBasePlayerCast* CastObject, class ABaseSpell* Spell);
	UFUNCTION()
	void OnSpellHit(class UBasePlayerCast* CastObject, class ABaseSpell* Spell, AActor* HitActor);
	UFUNCTION()
	void OnTriggeredSpellHit(class ABaseSpell* Spell, AActor* HitActor);
	UFUNCTION()
	void OnBasicAttackCastStarted(class UBasePlayerCast* CastObject);
	UFUNCTION()
	void OnBasicAttackCasted(class UBasePlayerCast* CastObject, class ABaseSpell* Spell);
	UFUNCTION()
	void OnBasicAttackHit(class UBasePlayerCast* CastObject, class ABaseSpell* Spell, AActor* HitActor);

	bool bCastingSpell = false;
	bool bCastingBasicAttack = false;
	int CurrentSpellId = 0;

	UPROPERTY(EditDefaultsOnly)
	TSet<WizardElement> LearnedElements { WizardElement::Fire };

	UPROPERTY(EditDefaultsOnly, Instanced)
	class UBasePlayerCast* BasicAttackCast;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpellConfig> SpellConfiguration;
	UPROPERTY(EditDefaultsOnly)
	TArray<FSpellCastConfig> SpellCastConfiguration;
	UPROPERTY(Transient)
	TMap<int, TSubclassOf<class ABaseSpell>> Spells;
	UPROPERTY(Transient)
	TMap<int, class UBasePlayerCast*> SpellCastObjects;
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

	UPROPERTY(Transient)
	class UActorPoolingSubsystem* PoolingSystem;
};
