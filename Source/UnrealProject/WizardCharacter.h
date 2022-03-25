// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Spells/FlameColumn.h"
#include "Spells/IceZone.h"
#include "Spells/IceWall.h"
#include "Spells/Tornado.h"
#include "Spells/ChainLightning.h"
#include "Spells/Shockwave.h"
#include <Components/BillboardComponent.h>
#include "FloatingTextActor.h"
#include "Enemies/BaseCharacter.h"

#include "WizardCharacter.generated.h"

UENUM()
enum class WizardElement
{
	Fire = 0,
	Frost = 1,
	Wind = 3
};

class UPowerUpEffect;

UCLASS()
class UNREALPROJECT_API AWizardCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWizardCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TakeSpellDamage(float damage) override;
	void TakeTickDamage(float damage) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnAddElement();

	UFUNCTION(BlueprintCallable)
	TArray<int> GetCurrentElements();

	UFUNCTION(BlueprintCallable)
	TMap<int, float>& GetCooldowns();
	UFUNCTION(BlueprintCallable)
	TMap<int, float>& GetCooldownCounters();

	void AddPowerUpEffect( UPowerUpEffect* effect);

private:
	void MoveUp(float value);
	void MoveRight(float value);
	void Fire(float input); //Base Projectile
	void CastSpell();

	void Dash();

	void UpdatePowerups(float deltaTime);

	UPROPERTY(EditDefaultsOnly)
	float m_DashForce = 10000.f;

	void CastFlameColumn(const FVector& worldPos);
	void CastIceZone(const FVector& worldPos);
	void CastIceWall(const FVector& worldPos);
	void CastTornado(const FVector& direction);
	void CastChainLightning(const FVector& direction);
	void CastShockwave();

	void AddElement(WizardElement element);
	DECLARE_DELEGATE_OneParam(FAddElementDelegate, WizardElement);


	UPROPERTY(EditAnywhere)
	USpringArmComponent* m_SpringArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* m_Camera;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_Mesh;

	UPROPERTY(EditDefaultsOnly)
	TArray<WizardElement> m_CurrentElements{};
	int m_MaxElements = 2;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<AFlameColumn> m_FlameColumnClass = nullptr;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<AIceZone> m_IceZoneClass = nullptr;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<AIceWall> m_IceWallClass = nullptr;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<ATornado> m_TornadoClass = nullptr;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<AChainLightning> m_ChainLighningClass = nullptr;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSubclassOf<AShockwave> m_ShockwaveClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int m_SpeedPerWind = 10;
	UPROPERTY(EditDefaultsOnly)
	int m_DamagePerFire = 10;
	UPROPERTY(EditDefaultsOnly)
	int m_DamageReductionPerFrost = 10;
	UPROPERTY(EditDefaultsOnly)
	int m_BaseDamageMultiplier = 20;
	UPROPERTY(EditDefaultsOnly)
	int m_DamageTakenMultiplier = 0;

	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* m_FirstElementBillboard = nullptr; 
	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* m_SecondElementBillboard = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* m_FireElementTexture = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* m_FrostElementTexture = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* m_WindElementTexture = nullptr;

	// Cooldowns
	UPROPERTY(EditAnywhere)
	TMap<int, float> m_Cooldowns{};
	UPROPERTY(VisibleAnywhere)
	TMap<int, float> m_CooldownCounter{};

	
	UPROPERTY(EditDefaultsOnly)
	float m_BasicAttackCooldown = 0.5f;
	UPROPERTY(VisibleAnywhere)
	float m_BasicAttackTimer = m_BasicAttackCooldown;

	UPROPERTY(VisibleAnywhere)
	TArray<UPowerUpEffect*> PowerUpEffects;

	friend class UPowerUpEffect;
};
