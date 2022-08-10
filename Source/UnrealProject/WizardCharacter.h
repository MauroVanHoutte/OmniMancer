// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/BaseCharacter.h"
#include "GenericPlatform/GenericPlatformMisc.h" 

#include "WizardCharacter.generated.h"

UENUM(BlueprintType)
enum class WizardElement : uint8
{
	Fire = 0,
	Frost = 1,
	Wind = 3
};

class UPowerUpEffect;
class UBillboardComponent;
class UCameraComponent;
class USpringArmComponent;
class AFlameColumn;
class AIceZone;
class AIceWall;
class ATornado;
class AChainLightning;
class AShockwave;
class ABaseProjectile;
class ABaseSpell;
class UUserWidget;

namespace TriggerEffects
{
	class BaseTriggerEffect;
}

UCLASS()
class UNREALPROJECT_API AWizardCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AWizardCharacter();
	AWizardCharacter(FVTableHelper& Helper);
	~AWizardCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void TakeSpellDamage(ABaseSpell* spell);
	virtual void TakeSpellDamageFloat(float damage, AActor* cause);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnSpellHitEnemy( ABaseSpell* spell, AActor* enemy);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAddElement();

	UFUNCTION(BlueprintCallable)
	const TArray<WizardElement>& GetCurrentElements();

	UFUNCTION(BlueprintCallable)
	void LevelUpElement(WizardElement element);

	UFUNCTION(BlueprintCallable)
	int GetCurrentElementLevel(WizardElement element);

	UFUNCTION(BlueprintCallable)
	TMap<int, float>& GetCooldowns();
	UFUNCTION(BlueprintCallable)
	TMap<int, FTimerHandle>& GetCooldownTimers();

	void AddPowerUpEffect( UPowerUpEffect* effect);

	void SetExplosionVariables(float damage, float radius, bool explode);
	void GetExplosionVariables(float& damageOut, float& radiusOut);
	void SetSpread(int spread);
	int GetSpread();
	void SetBasicAttackCooldown(float fireRate);
	float GetBasicAttackCooldown();
	void SetSpeed(float speed);
	float GetSpeed();
	void SetBounces(int bounces);
	int GetBounces();
	float GetSpellDamageMultiplier();
	void SetSpellDamageMultiplier(float newSpellDamage);
	float GetDamageTakenMultiplier();
	void SetDamageTakenMultiplier(float newDamageTaken);
	TArray<FStatusEffect>& GetBaseAttackEffectsRef();

	UFUNCTION(BlueprintCallable)
	int GetCombinedElementLevel();

	void LowerCooldowns(float amount);

protected:
	virtual void BeginPlay() override;

	void TakeTickDamage(float damage) override;

	virtual void OnTakeHit(AActor* actor);

	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	int FireLevel = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	int FrostLevel = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	int WindLevel = 1;

private:
	void Pause();
	void MoveUp(float value);
	void MoveRight(float value);
	void Fire(float input); //Base Projectile
	void InitProjectile(ABaseProjectile* projectile, const FVector& direction);
	void CastSpell();

	void Dash();

	void UpdatePowerups(float deltaTime);

	void SetupMainElementPassive();

	UPROPERTY(EditDefaultsOnly)
	float DashForce = 10000.f;
	UPROPERTY(EditDefaultsOnly)
	float DashCooldown = 5.f;
	FTimerHandle DashCooldownTimer;

	void AddElement(WizardElement element);
	DECLARE_DELEGATE_OneParam(FAddElementDelegate, WizardElement);


	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WizardMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	TArray<WizardElement> CurrentElements{};
	int MaxElements = 2;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	TSubclassOf<ABaseProjectile> BaseProjectile = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	TMap<int, TSubclassOf<ABaseSpell>> Spells;
	//percentage increase
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	float SpellDamageMultiplier = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	float SlowDurationMultiplier = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	float StunDurationMultiplier = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	float BurnDurationMultiplier = 1;


	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	WizardElement MainElement = WizardElement::Fire;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	float SpeedPerWind = 0.1f;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	float DamagePerFire = 0.1f;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	float DamageReductionPerFrost = 0.1f;
	//Percentage Increase
	UPROPERTY(VisibleAnywhere)
	float BaseDamageMultiplier = 1;
	//Percentage Reduction
	UPROPERTY(VisibleAnywhere)
	float DamageTakenMultiplier = 1;
	

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UBillboardComponent* FirstElementBillboard = nullptr; 
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UBillboardComponent* SecondElementBillboard = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UTexture2D* FireElementTexture = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UTexture2D* FrostElementTexture = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UTexture2D* WindElementTexture = nullptr;

	// Cooldowns
	UPROPERTY(EditAnywhere, Category = "Spells")
	TMap<int, float> Cooldowns{};
	UPROPERTY(VisibleAnywhere, Category = "Spells")
	TMap<int, FTimerHandle> CooldownTimers{};


	
	UPROPERTY(EditDefaultsOnly)
	float BasicAttackCooldown = 0.5f;
	FTimerHandle BasicAttackTimer;

	//Powerups
	UPROPERTY(VisibleAnywhere, Category = "Active")
	TArray<UPowerUpEffect*> PowerUpEffects;

	UPROPERTY(VisibleAnywhere, Category = "Active")
	TArray<FStatusEffect> BaseAttackEffects;

	//effects that are applied to attacker when wizard takes damage
	UPROPERTY(VisibleAnywhere, Category = "Active")
	TArray<FStatusEffect> ReflectEffects;
	TArray<TUniquePtr<TriggerEffects::BaseTriggerEffect>> OnCastTriggers;
	TArray<TUniquePtr<TriggerEffects::BaseTriggerEffect>> OnHitTriggers;
	TArray<TUniquePtr<TriggerEffects::BaseTriggerEffect>> OnTakeHitTriggers;


	int Spread = 0;

	int Bounces = 0;

	bool ExplosiveBaseAttack = false;
	float ExplosionRadius = 0;
	float ExplosionDamage = 0;

};
