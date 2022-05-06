// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../StatusEffect.h"

#include "BaseCharacter.generated.h"

class APowerUp;
class UNiagaraComponent;
class AFloatingTextActor;

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

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	TArray<FStatusEffect> CurrentStatusEffects{};

	UPROPERTY(EditAnywhere)
	float Health;
	UPROPERTY(VisibleAnywhere)
	float StartHealth;

	UPROPERTY(VisibleAnywhere)
	float SlowAmount = 0;

	UPROPERTY(VisibleAnywhere)
	bool Stunned = false;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* NiagaraComponent = nullptr;

	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFloatingTextActor> FloatingTextClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FColor DamageTextColor = FColor::White;

	UPROPERTY(EditDefaultsOnly)
	bool SpawnPowerupOnDeath = true;
	UPROPERTY(EditDefaultsOnly)
	float PowerupSpawnChance = 10;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APowerUp> Powerup;

};
