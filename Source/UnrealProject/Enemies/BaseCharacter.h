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

class APowerUp;

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

	UFUNCTION(BlueprintCallable)
	virtual void TakeSpellDamage(float damage);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CheckDeath();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	virtual void TakeTickDamage(float damage);
	virtual void AddStatusEffects(const TArray<FStatusEffect>& statusEffects);
	void SpawnDamageText(float damage);

	void SpawnPowerup();

	void UpdateStatusEffects(float deltaTime);

	UPROPERTY(VisibleAnywhere)
	TArray<FStatusEffect> m_CurrentStatusEffects{};

	UPROPERTY(VisibleAnywhere)
	float m_Health;
	UPROPERTY(EditDefaultsOnly)
	float m_StartHealth;

	UPROPERTY(VisibleAnywhere)
	float m_SlowAmount = 0;

	UPROPERTY(VisibleAnywhere)
	bool m_Stunned = false;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* m_NiagaraComponent = nullptr;

	UCharacterMovementComponent* m_CharacterMovement;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFloatingTextActor> m_FloatingTextClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FColor m_DamageTextColor = FColor::White;

	UPROPERTY(EditDefaultsOnly)
	bool m_SpawnPowerupOnDeath = true;
	UPROPERTY(EditDefaultsOnly)
	float m_PowerupSpawnChance = 10;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APowerUp> m_Powerup;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
