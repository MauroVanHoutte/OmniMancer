// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../StatusEffect.h"
#include "../FloatingTextActor.h"
#include <Components/WidgetComponent.h>
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "BaseCharacter.generated.h"

UCLASS(Abstract)
class UNREALPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	virtual void ReapplyStatusEffects(const TArray<FStatusEffect>& statusEffects);
	virtual void Knockup();
	virtual void Push(const FVector& force);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStunned();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();
	UFUNCTION(BlueprintCallable)
	bool GetStunned();
	UFUNCTION(BlueprintCallable)
	float GetHealth();
	UFUNCTION(BlueprintCallable)
	float GetStartHealth();

	UFUNCTION(BlueprintCallable)
	void Heal(float hp);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CheckDeath();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void TakeSpellDamage(float damage);
	virtual void TakeTickDamage(float damage);
	virtual void AddStatusEffects(const TArray<FStatusEffect>& statusEffects);
	void SpawnDamageText(float damage);

	UPROPERTY(EditAnywhere)
	TArray<FStatusEffect> m_CurrentStatusEffects{};

	UPROPERTY(EditAnywhere)
	float m_Health;
	float m_StartHealth;

	UPROPERTY(EditAnywhere)
	float m_SlowAmount = 0;

	UPROPERTY(EditAnywhere)
	bool m_Stunned = false;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* m_NiagaraComponent = nullptr;

	UCharacterMovementComponent* m_CharacterMovement;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AFloatingTextActor> m_FloatingTextClass = nullptr;

	UPROPERTY(EditAnywhere)
	FColor m_DamageTextColor = FColor::White;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
