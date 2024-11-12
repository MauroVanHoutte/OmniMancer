// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatusEffects/StatusEffect.h"

#include "BaseCharacter.generated.h"

class APowerUp;
class UNiagaraComponent;
class AFloatingTextActor;
class ABaseSpell;
class ACoin;

UCLASS(Abstract)
class UNREALPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	//ABaseCharacter();
	//virtual void ReapplyStatusEffects(const TArray<FStatusEffect>& statusEffects);
	//virtual void AddStatusEffect(const FStatusEffect& effect);
	//virtual void AddStatusEffects(const TArray<FStatusEffect>& statusEffects);

	virtual void Knockup();
	virtual void Push(const FVector& force);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnStunned();
	UFUNCTION(BlueprintImplementableEvent)
	void OnStunnedEnd();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeathEvent();
	UFUNCTION(BlueprintCallable)
	bool GetStunned() const;
	UFUNCTION(BlueprintCallable)
	float GetHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetStartHealth() const;

	UFUNCTION(BlueprintCallable)
	void Heal(float hp);

	UFUNCTION(BlueprintCallable)
	virtual void TakeSpellDamage(ABaseSpell* spell);
	//no on hit effects can be applied
	UFUNCTION(BlueprintCallable)
	virtual void TakeSpellDamageFloat(float damage, AActor* cause);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CheckDeath();

	UFUNCTION(BlueprintCallable)
	virtual void OnDeath();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void OnTakeHit(AActor* cause);
	UFUNCTION(BlueprintImplementableEvent)
	void OnTakeHitBP();

	virtual void TakeTickDamage(float damage);
	void SpawnDamageText(float damage);

	UFUNCTION(BlueprintImplementableEvent)
	void PerceiveDamage(AActor* cause, const FVector& location);

	void SpawnPowerup() const;
	void SpawnCoins() const;

	//void UpdateStatusEffects(float deltaTime);

	//UPROPERTY(VisibleAnywhere)
	//TArray<FStatusEffect> CurrentStatusEffects{};

	UPROPERTY(VisibleAnywhere)
	float Health;
	UPROPERTY(EditAnywhere)
	float StartHealth;

	UPROPERTY(VisibleAnywhere)
	float SlowAmount = 0;

	UPROPERTY(VisibleAnywhere)
	bool Stunned = false;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* NiagaraComponent = nullptr;

	UCharacterMovementComponent* CharacterMovement;

	//Popup text
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFloatingTextActor> FloatingTextClass = nullptr;
	UPROPERTY(EditDefaultsOnly)
	FColor DamageTextColor = FColor::White;

	//Powerups
	UPROPERTY(EditDefaultsOnly)
	bool SpawnPowerupOnDeath = true;
	UPROPERTY(EditDefaultsOnly)
	float PowerupSpawnChance = 0.1f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APowerUp> PowerupBP;

	//knockback
	UPROPERTY(EditDefaultsOnly)
	float KnockbackMultiplier = 1.f;

	//Coins
	UPROPERTY(EditDefaultsOnly)
	int MinCoinsDropped = 0;
	UPROPERTY(EditDefaultsOnly)
	int MaxCoinsDropped = 2;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACoin> CoinBP;

private:
	//void SpreadCurse(const FStatusEffect& curseEffect);

};
