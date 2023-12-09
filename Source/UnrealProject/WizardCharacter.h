// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/BaseCharacter.h"
#include "GenericPlatform/GenericPlatformMisc.h" 
#include "Upgrades/CharacterUpgrades.h"
#include "SpellCasting/Elements.h"

#include "WizardCharacter.generated.h"

//UENUM(BlueprintType)
//enum class WizardElement : uint8
//{
//	// x = 2^n - 1
//	Fire = 0,
//	Frost = 1,
//	Wind = 3
//};

UENUM(BlueprintType)
enum class TriggerCondition : uint8
{
	OnCast,
	OnHit,
	OnTakeHit
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
class UPlayerHUD;
class UBaseTriggerEffect;


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

	void OnSpellHitEnemy( ABaseSpell* spell, ABaseCharacter* enemy);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAddElement();

	UFUNCTION(BlueprintCallable)
	const TArray<WizardElement>& GetCurrentElements() const;

	UFUNCTION(BlueprintCallable)
	void LevelUpElement(WizardElement element);

	UFUNCTION(BlueprintCallable)
	int GetCurrentElementLevel(WizardElement element) const;

	UFUNCTION(BlueprintCallable)
	const TMap<int, float>& GetCooldowns() const;
	UFUNCTION(BlueprintCallable)
	const TMap<int, FTimerHandle>& GetCooldownTimers() const;

	//Powerups/upgrades
	void AddPowerUpEffect( UPowerUpEffect* effect);
	void AddTriggerEffect(UBaseTriggerEffect* effect);
	void RemoveTriggerEffect(UBaseTriggerEffect* effect);
	UFUNCTION(BlueprintCallable)
	void ApplyUpgrade(const FString& tag);
	UFUNCTION(BlueprintCallable)
	void RemoveUpgrade(const FString& tag);

	//Getters/Setters
	const FVector& GetDesiredDirection() const;
	void SetExplosionVariables(float damage, float radius, bool explode);
	void GetExplosionVariables(float& damageOut, float& radiusOut) const;
	void SetSpread(int spread);
	int GetSpread() const;
	void SetBasicAttackCooldown(float fireRate);
	float GetBasicAttackCooldown() const;
	void SetSpeed(float speed);
	float GetSpeed() const;
	void SetBounces(int bounces);
	int GetBounces() const;
	float GetSpellDamageMultiplier() const;
	void SetSpellDamageMultiplier(float newSpellDamage);
	float GetBaseProjecileDamageMultiplier() const;
	void SetBaseProjectileDamageMultiplier(float newBaseProjectileMultiplier);
	float GetDamageTakenMultiplier() const;
	void SetDamageTakenMultiplier(float newDamageTaken);
	float GetBurnDurationMultiplier() const;
	void SetBurnDurationMultiplier(float newBurnDurationMultiplier);
	float GetSlowDurationMultiplier() const;
	void SetSlowDurationMultiplier(float newSlowDurationMultiplier);
	float GetStunDurationMultiplier() const;
	void SetStunDurationMultiplier(float newStunDurationMultiplier);
	float GetDashForce() const;
	void SetDashForce(float newDashForce);
	void AddBaseAttackStatusEffect(const FStatusEffect& effect);
	void RemoveBaseAttackStatusEffect(const FStatusEffect& effect);
	void AddReflectStatusEffect(const FStatusEffect& effect);
	void RemoveReflectStatusEffect(const FStatusEffect& effect);
	TArray<FStatusEffect>& GetBaseAttackEffectsRef();
	float GetExperiencePercentage() const;

	void PickupExperience(int experience);

	UFUNCTION(BlueprintImplementableEvent)
	void OnLevelUp();

	UFUNCTION(BlueprintCallable)
	int GetCombinedElementLevel() const;

	void LowerCooldowns(float amount);

protected:
	virtual void BeginPlay() override;

	void TakeTickDamage(float damage) override;

	virtual void OnTakeHit(AActor* actor);
	virtual void OnDeath() override;


	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	int FireLevel = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	int FrostLevel = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	int WindLevel = 1;

	UInputComponent* InputComponent = nullptr;

private:
	void Pause();
	void ShowMap();
	void HideMap();
	void MoveUp(float value);
	void MoveRight(float value);
	void Fire(float input); //Base Projectile
	void SpawnSpreadProjectiles(FActorSpawnParameters& spawnParams, FVector& direction);
	void InitProjectile(ABaseProjectile* projectile, const FVector& direction);
	void CastSpell();

	void ExecuteTriggerEffects(const TriggerCondition& condition, AWizardCharacter* caster, ABaseSpell* spell, ABaseCharacter* target);

	void Dash();

	void UpdatePowerups(float deltaTime);

	void SetupUpgrades();

	void RaycastMouseOnLevel(FVector& mouseAtWizardHeight, FHitResult& raycastHit);

	void AddElement(WizardElement element);
	DECLARE_DELEGATE_OneParam(FAddElementDelegate, WizardElement);

	//Dash
	UPROPERTY(EditDefaultsOnly)
	float DashForce = 10000.f;
	UPROPERTY(EditDefaultsOnly)
	float DashCooldown = 5.f;
	FTimerHandle DashCooldownTimer;

	//Camera
	UPROPERTY(EditDefaultsOnly)
	float SpringArmLength = 1200.f;
	UPROPERTY(EditDefaultsOnly)
	float SpringArmZoomedOutLength = 10000.f;
	float TargetSpringArmLength = SpringArmLength;
	UPROPERTY(EditDefaultsOnly)
	float CameraZoomSpeed = 20000.f;
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	//Active elements
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	TArray<WizardElement> CurrentElements{};
	int MaxElements = 2;
	
	//Spell bp's
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	TSubclassOf<ABaseProjectile> BaseProjectile = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	TMap<int, TSubclassOf<ABaseSpell>> Spells;

	//Multipliers
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	float SpellDamageMultiplier = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	float SlowDurationMultiplier = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	float StunDurationMultiplier = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Spells")
	float BurnDurationMultiplier = 1;
	UPROPERTY(VisibleAnywhere)
	float BaseDamageMultiplier = 1;
	UPROPERTY(VisibleAnywhere)
	float DamageTakenMultiplier = 1;

	//Active element passice effects
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	WizardElement MainElement = WizardElement::Fire;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	float SpeedPerWind = 0.1f;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	float DamagePerFire = 0.1f;
	UPROPERTY(EditDefaultsOnly, Category = "Elements")
	float DamageReductionPerFrost = 0.1f;
	
	//Active element visuals
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals", meta = (AllowPrivateAccess = true))
	UBillboardComponent* FirstElementBillboard = nullptr; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals", meta = (AllowPrivateAccess = true))
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
	//effects applied on hit, take hit and cast
	TArray<UBaseTriggerEffect*> TriggerEffects;
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UCharacterUpgrade*> CharacterUpgrades;

	//Experience
	int Experience = 0;
	UPROPERTY(EditDefaultsOnly)
	int ExeperienceRequired = 100;
	UPROPERTY(EditDefaultsOnly)
	float NextLevelExperienceRequired = 1.1f;

	//base attack effects
	int Spread = 0;

	int Bounces = 0;

	bool ExplosiveBaseAttack = false;
	float ExplosionRadius = 0;
	float ExplosionDamage = 0;

	//UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerHUD> PlayerHudType = nullptr;
	UPlayerHUD* PlayerHud;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PauseMenuType;
	UUserWidget* PauseMenu;
};
