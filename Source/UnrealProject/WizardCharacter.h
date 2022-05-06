// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/BaseCharacter.h"

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

UCLASS()
class UNREALPROJECT_API AWizardCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AWizardCharacter();

protected:
	virtual void BeginPlay() override;

	void TakeSpellDamage(float damage) override;
	void TakeTickDamage(float damage) override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnAddElement();

	UFUNCTION(BlueprintCallable)
	const TArray<WizardElement>& GetCurrentElements();

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

private:
	void MoveUp(float value);
	void MoveRight(float value);
	void Fire(float input); //Base Projectile
	void InitProjectile(ABaseProjectile* projectile, const FVector& direction);
	void CastSpell();

	void Dash();

	void UpdatePowerups(float deltaTime);

	UPROPERTY(EditDefaultsOnly)
	float m_DashForce = 10000.f;
	UPROPERTY(EditDefaultsOnly)
	float m_DashCooldown = 5.f;
	FTimerHandle m_DashCooldownTimer;

	void CastFlameColumn(const FVector& worldPos);
	void CastIceZone(const FVector& worldPos);
	void CastIceWall(const FVector& worldPos);
	void CastTornado(const FVector& direction);
	void CastChainLightning(const FVector& direction);
	void CastShockwave();

	void AddElement(WizardElement element);
	DECLARE_DELEGATE_OneParam(FAddElementDelegate, WizardElement);


	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WizardMesh;

	UPROPERTY(EditDefaultsOnly)
	TArray<WizardElement> CurrentElements{};
	int MaxElements = 2;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<AFlameColumn> FlameColumnClass = nullptr;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<AIceZone> IceZoneClass = nullptr;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<AIceWall> IceWallClass = nullptr;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<ATornado> TornadoClass = nullptr;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<AChainLightning> ChainLighningClass = nullptr;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<AShockwave> ShockwaveClass = nullptr;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<ABaseProjectile> BaseProjectile = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int SpeedPerWind = 10;
	UPROPERTY(EditDefaultsOnly)
	int DamagePerFire = 10;
	UPROPERTY(EditDefaultsOnly)
	int DamageReductionPerFrost = 10;
	UPROPERTY(EditDefaultsOnly)
	int BaseDamageMultiplier = 20;
	UPROPERTY(EditDefaultsOnly)
	int DamageTakenMultiplier = 0;

	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* FirstElementBillboard = nullptr; 
	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* SecondElementBillboard = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* FireElementTexture = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* FrostElementTexture = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* WindElementTexture = nullptr;

	// Cooldowns
	UPROPERTY(EditAnywhere)
	TMap<int, float> Cooldowns{};
	UPROPERTY(VisibleAnywhere)
	TMap<int, FTimerHandle> CooldownTimers{};

	
	UPROPERTY(EditDefaultsOnly)
	float BasicAttackCooldown = 0.5f;
	FTimerHandle BasicAttackTimer;

	//Powerups
	UPROPERTY(VisibleAnywhere)
	TArray<UPowerUpEffect*> PowerUpEffects;

	TArray<FStatusEffect> BaseAttackEffects;

	int Spread = 0;

	int Bounces = 0;

	bool ExplosiveBaseAttack = false;
	float ExplosionRadius = 0;
	float ExplosionDamage = 0;

};
