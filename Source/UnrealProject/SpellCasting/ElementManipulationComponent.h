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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBasicAttackCastedSignature, AActor*, Caster, ABaseSpell*, Spell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellHitSignature, ABaseSpell*, Spell, AActor*, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBasicAttackHitSignature, ABaseSpell*, Spell, AActor*, HitActor);


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
	void Initialize(class APlayerController* Controller, class UBillboardComponent* FirstElementBillboard, class UBillboardComponent* SecondElementBillboard, class UStatComponent* StatComponent);

	UFUNCTION(BlueprintCallable)
	void AddElement(WizardElement Element);

	UFUNCTION(BlueprintCallable)
	void TryCastSpell();

	UFUNCTION(BlueprintCallable)
	void TryCastBasicAttack();

	UPROPERTY(BlueprintAssignable)
	FElementAddedSignature OnElementAddedDelegate;
	UPROPERTY(BlueprintAssignable)
	FSpellCastedSignature OnSpellCastedDelegate;
	UPROPERTY(BlueprintAssignable)
	FBasicAttackCastedSignature OnBasicAttackCastedDelegate;
	UPROPERTY(BlueprintAssignable)
	FSpellHitSignature OnSpellHitDelegate;
	UPROPERTY(BlueprintAssignable)
	FBasicAttackHitSignature OnBasicAttackHitDelegate;

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
	float CalculateBaseAttackCooldown();
	float CalculateSpellCooldown(const TSubclassOf<class ABaseSpell>* Spell);

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpellConfig> SpellConfiguration;
	UPROPERTY()
	TMap<int, TSubclassOf<class ABaseSpell>> Spells;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseSpell> BasicAttack;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<class ABaseSpell>, float> Cooldowns{};
	UPROPERTY(VisibleAnywhere)
	TMap<TSubclassOf<class ABaseSpell>, FTimerHandle> CooldownTimers{};

	UPROPERTY(Transient)
	class APlayerController* Controller;
	UPROPERTY(Transient)
	class UStatComponent* Stats;

	UPROPERTY(EditDefaultsOnly)
	TArray<WizardElement> CurrentElements;
	UPROPERTY(EditDefaultsOnly)
	TMap<WizardElement, class UTexture2D*> ElementTextures;
	UPROPERTY(Transient, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UBillboardComponent* FirstElementBillboard;
	UPROPERTY(Transient, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UBillboardComponent* SecondElementBillboard;
};
