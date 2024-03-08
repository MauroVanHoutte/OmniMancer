// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(class UCharacterMovementComponent* MovementComponent);

	//BaseAttack
	UFUNCTION(BlueprintCallable)
	void SetBaseAttackSpread(int newSpread);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetBaseAttackSpread();
	UFUNCTION(BlueprintCallable)
	void SetBaseAttackBounces(int newBounces);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetBaseAttackBounces();
	UFUNCTION(BlueprintCallable)
	void SetBaseAttackCooldownMultiplier(float newMultiplier);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBaseAttackCooldownMultiplier();
	UFUNCTION(BlueprintCallable)
	void SetBaseAttackDamageMultiplier(float newMultiplier);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBaseAttackDamageMultiplier();
	//~BaseAttack

	//Spells
	UFUNCTION(BlueprintCallable)
	void SetSpellCooldownMultiplier(float newSpellCooldownMultiplier);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSpellCooldownMultiplier();
	UFUNCTION(BlueprintCallable)
	void SetSpellDamageMultiplier(float newSpellDamageMultiplier);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSpellDamageMultiplier();
	//~Spells

	//Mobility
	UFUNCTION(BlueprintCallable)
	void SetSpeedMultiplier(float newSpeedMultiplier);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSpeedMultiplier();
	UFUNCTION(BlueprintCallable)
	void SetDashForceMultiplier(float newDashForceMultiplier);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDashForceMultiplier();
	UFUNCTION(BlueprintCallable)
	void SetDashCooldownMultiplier(float newDashCooldownMultiplier);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDashCooldownMultiplier();
	//~Mobility

private:

	//BaseAttack
	int BaseAttackSpread = 0;
	int BaseAttackBounces = 0;
	float BaseAttackCooldownMultiplier = 1.f;
	float BaseAttackDamageMultiplier = 1.f;
	//~BaseAttack

	//Spells
	float SpellCooldownMultiplier = 1.f;
	float SpellDamageMultiplier = 1.f;
	//~Spells

	//Mobility
	float MovementSpeedMultiplier = 1.f;
	float DefaultMovementSpeed = 0;
	float DashForceMultiplier = 1.f;
	float DashCooldownMultiplier = 1.f;
	UPROPERTY(Transient)
	class UCharacterMovementComponent* CharacterMovementComponent;
	//~Mobility
};
