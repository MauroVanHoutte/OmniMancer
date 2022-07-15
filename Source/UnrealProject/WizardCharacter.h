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

	void OnBaseProjectileHitEnemy(AActor* enemy);

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
	TArray<FStatusEffect>& GetBaseAttackEffectsRef();

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
	void MoveUp(float value);
	void MoveRight(float value);
	void Fire(float input); //Base Projectile
	void InitProjectile(ABaseProjectile* projectile, const FVector& direction);
	void CastSpell();

	void Dash();

	void UpdatePowerups(float deltaTime);

	void SetupMainElementPassive();

	UPROPERTY(EditDefaultsOnly)
	float m_DashForce = 10000.f;
	UPROPERTY(EditDefaultsOnly)
	float m_DashCooldown = 5.f;
	FTimerHandle m_DashCooldownTimer;

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

	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	WizardElement MainElement = WizardElement::Fire;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	int SpeedPerWind = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	int DamagePerFire = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	int DamageReductionPerFrost = 10;
	UPROPERTY(EditDefaultsOnly)
	int BaseDamageMultiplier = 20;
	UPROPERTY(EditDefaultsOnly)
	int DamageTakenMultiplier = 0;

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


	int Spread = 0;

	int Bounces = 0;

	bool ExplosiveBaseAttack = false;
	float ExplosionRadius = 0;
	float ExplosionDamage = 0;

};
